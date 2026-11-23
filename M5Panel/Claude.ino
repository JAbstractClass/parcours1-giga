// ---------- Claude: Sprachsteuerung ----------
// Sprechtaste am Giga halten -> Giga schickt das Mikrofon per MQTT (p1/audio/...) hierher.
// Das M5 streamt die Aufnahme verschluesselt an den Server (jl-parcours, POST /stimme, chunked),
// zeigt dann Claudes Zwischenschritte live, liest die Antwort vor und fragt bei Code-Aenderungen,
// ob sie uebernommen werden sollen (C = ja -> main -> GitHub baut -> Giga holt sich die Firmware).
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Zertifikate.h"
#include <time.h>

// Nur X1: mit zusaetzlich X2 in der Liste scheitert die Pruefung auf dem ESP32 (getestet); die Kette
// des Servers enthaelt X2 quer-signiert von X1, das reicht.
String caListe() { return String(ISRG_ROOT_X1_PEM); }
String CA_LISTE;

// Zertifikate werden nach Datum geprueft: ohne Uhrzeit (1970) waeren sie "noch nicht gueltig"
bool uhrOk() { return time(nullptr) > 1700000000; }
void uhrStellen() {                              // NTP; bis es klappt alle 30 s neu versuchen
  static unsigned long t = 0;
  if (uhrOk() || (t && millis() - t < 30000)) return;
  t = millis(); configTime(0, 0, "pool.ntp.org", "time.google.com", "time.cloudflare.com");
}
// Uhr vom Giga uebernehmen (der hat sie per NTP fuer seine GitHub-Abfragen)
void uhrVomGiga(uint32_t unix) {
  if (unix < 1700000000 || abs((long)(time(nullptr) - unix)) < 60) return;
  struct timeval tv = {(time_t)unix, 0}; settimeofday(&tv, nullptr);
}

enum KZustand { K_AUS, K_HOERT, K_SENDET, K_DENKT, K_SPRICHT, K_FERTIG, K_FEHLER };
volatile KZustand kz = K_AUS;
String kFrage, kAntwort, kAenderung, kMeldung, kJob;
std::vector<String> kSchritte;                  // geschuetzt durch mtx
int kN = 0;
volatile int kPegel = 0;
volatile char kAktion = 0;                       // 'j' uebernehmen, 'n' verwerfen, 'w' Antwort wiederholen
volatile bool kZeigen = false;                   // Hauptschleife soll auf die Claude-Seite wechseln
volatile bool kTonStopp = false;
unsigned long kSeit = 0;

String p1Host() { String u = P1_URL; u.replace("https://", ""); return u.substring(0, u.indexOf('/')); }
String p1Pfad() { String u = P1_URL; u.replace("https://", ""); return u.substring(u.indexOf('/')); }

// Diagnose per MQTT (p1/claude), z. B. mit einem MQTT-Client am Laptop mitlesen
void kLog(const String& s) { senden("p1/claudelog", String(millis()) + " " + s + " heap " + ESP.getFreeHeap() + " groesster Block " + heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)); }

void kSchritt(const String& s) { xSemaphoreTake(mtx, portMAX_DELAY); kSchritte.push_back(s); if (kSchritte.size() > 40) kSchritte.erase(kSchritte.begin()); xSemaphoreGive(mtx); }

WiFiClientSecure kCli;                           // Verbindung fuer Abfragen und Vorlesen

// Zustand der Anlage als kurze JSON-Zeile fuer Claude (aus dem zuletzt empfangenen Status)
String statusFuerClaude() {
  String s; xSemaphoreTake(mtx, portMAX_DELAY); s = statusRoh; xSemaphoreGive(mtx);
  s.replace("\r", ""); s.replace("\n", "");
  return s.length() ? s : "{}";
}

// ---------- Aufnahme (macht der Giga) ----------
// Der Giga nimmt die ganze Aufnahme in seinem Speicher auf und schickt sie selbst an den Server
// (das M5 hat dafuer zu wenig RAM). Er meldet per MQTT: p1/audio/start, p1/audio/pegel,
// p1/claude/start (sendet), p1/claude/job (Job-Nummer) oder p1/claude/fehler. Alles im MQTT-Task,
// deshalb hier nur Variablen setzen - nie blockieren.
void upStart() {
  xSemaphoreTake(mtx, portMAX_DELAY); kSchritte.clear(); xSemaphoreGive(mtx);
  kFrage = ""; kAntwort = ""; kAenderung = ""; kMeldung = ""; kJob = ""; kN = 0;
  kz = K_HOERT; kSeit = millis(); kZeigen = true; kTonStopp = true;
}
void upPegel(const char* p) { kPegel = atoi(p); }
void upSendet(const char* p) { kz = K_SENDET; kSchritt(String("Aufnahme ") + String(atoi(p) / 8000.0, 1) + " s wird gesendet"); }
void upJob(const char* id) { kJob = id; kN = 0; kz = K_DENKT; kZeigen = true; }
void upFehler(const char* t) { kMeldung = t; kz = K_FEHLER; kZeigen = true; }
void upWache() {}

// ---------- Abfragen, Vorlesen, Uebernehmen (eigener Task) ----------
String p1Anfrage(const char* methode, const String& pfad, int* code = nullptr) {
  HTTPClient h;
  h.setReuse(false); h.setTimeout(15000);
  if (!h.begin(kCli, "https://" + p1Host() + p1Pfad() + pfad)) return "";
  h.addHeader("X-P1-Token", P1_TOKEN);
  int c = strcmp(methode, "POST") == 0 ? h.POST("") : h.GET();
  if (code) *code = c;
  String r = c > 0 ? h.getString() : "";
  h.end(); kCli.stop();
  return r;
}

void vorlesen() {
  kz = K_SPRICHT; kTonStopp = false;
  HTTPClient h;
  h.setTimeout(30000);
  if (!h.begin(kCli, "https://" + p1Host() + p1Pfad() + "/tts/" + kJob)) { kz = K_FERTIG; return; }
  h.addHeader("X-P1-Token", P1_TOKEN);
  int tc = h.GET();
  if (tc != 200) { char e[80] = ""; kCli.lastError(e, sizeof(e)); kLog("tts fehlgeschlagen " + String(tc) + " " + e); }
  if (tc == 200) {
    WiFiClient* s = h.getStreamPtr();
    int rest = h.getSize();
    static uint8_t puf[3][4096]; int k = 0;
    M5.Speaker.setVolume(230);
    while (rest > 0 && !kTonStopp) {
      int n = 0; unsigned long t0 = millis();
      while (n < (int)sizeof(puf[0]) && n < rest && millis() - t0 < 5000) {
        int a = s->available();
        if (a > 0) n += s->readBytes(puf[k] + n, min(a, min((int)sizeof(puf[0]) - n, rest - n)));
        else delay(2);
      }
      if (n <= 0) break;
      rest -= n;
      while (M5.Speaker.isPlaying(0) == 2 && !kTonStopp) delay(5);   // Warteschlange voll: warten
      M5.Speaker.playRaw(puf[k], n, 8000, false, 1, 0, false);
      k = (k + 1) % 3;
    }
    while (M5.Speaker.isPlaying(0) && !kTonStopp) delay(10);
    if (kTonStopp) M5.Speaker.stop(0);
    M5.Speaker.setVolume(140);
  }
  h.end(); kCli.stop();
  kz = K_FERTIG;
}

// ---------- Aktionen: Claude bedient die Anlage ----------
std::vector<String> kAktionen;
void aktionAusfuehren(const String& a) {
  auto warten = [](int ms) { vTaskDelay(pdMS_TO_TICKS(ms)); };
  kSchritt("Aktion: " + a);
  if (a == "aufzug_an")        { if (!S.aufzug) befehl('a'); }
  else if (a == "aufzug_aus")  { if (S.aufzug) befehl('a'); }
  else if (a == "freigeben")   befehl('s');
  else if (a == "weiche1_looping") { if (S.weiche == 2) befehl('w'); }
  else if (a == "weiche1_vorbei")  { if (S.weiche != 2) befehl('w'); }
  else if (a == "weiche2_spirale") { if (S.hebel == 2) befehl('h'); }
  else if (a == "weiche2_skurve")  { if (S.hebel != 2) befehl('h'); }
  else if (a == "licht_an")    { if (!S.leds) befehl('l'); }
  else if (a == "licht_aus")   { if (S.leds) befehl('l'); }
  else if (a == "servo_test")  befehl('v');
  else if (a == "weihnacht_an")  { if (!S.weihnacht) befehl('j'); }
  else if (a == "weihnacht_aus") { if (S.weihnacht) befehl('j'); }
  else if (a == "notaus")      befehl('x');
  else if (a.startsWith("warte_")) { warten(constrain(a.substring(6).toInt(), 0, 120) * 1000); return; }
  else if (a.startsWith("set_"))   { senden("p1/set", a.substring(4)); }
  warten(700);                                     // neuen Zustand abwarten, bevor die naechste Aktion prueft
}
void aktionenTask(void* p) {
  std::vector<String>* liste = (std::vector<String>*)p;
  for (auto& a : *liste) aktionAusfuehren(a);
  kSchritt("Aktionen fertig");
  delete liste;
  vTaskDelete(nullptr);
}

void claudeTask(void*) {
  if (!CA_LISTE.length()) CA_LISTE = caListe();
  kCli.setCACert(CA_LISTE.c_str());
  for (;;) {
    if (kz == K_DENKT && kJob.length()) {
      int code = 0;
      String r = p1Anfrage("GET", "/job/" + kJob + "?ab=" + kN, &code);
      if (code <= 0) { char e[80] = ""; kCli.lastError(e, sizeof(e)); kLog("abfrage fehlgeschlagen " + String(code) + " " + e); }
      JsonDocument j;
      if (r.length() && !deserializeJson(j, r)) {
        for (JsonVariant s : j["schritte"].as<JsonArray>()) kSchritt((const char*)s);
        kN = j["n"] | kN;
        const char* st = j["status"] | "";
        if (!strcmp(st, "fertig") || !strcmp(st, "fehler")) {
          kFrage = (const char*)(j["frage"] | "");
          kAntwort = (const char*)(j["antwort"] | "");
          kAenderung = (const char*)(j["aenderung"] | "");
          if (!strcmp(st, "fehler")) kMeldung = (const char*)(j["fehler"] | "");
          auto* liste = new std::vector<String>();
          for (JsonVariant a : j["aktionen"].as<JsonArray>()) liste->push_back((const char*)a);
          if (liste->size()) xTaskCreatePinnedToCore(aktionenTask, "aktionen", 6144, liste, 1, nullptr, 1);   // parallel zum Vorlesen
          else delete liste;
          vorlesen();
          if (kMeldung.length()) kz = K_FEHLER;
        } else if (!kFrage.length()) kFrage = (const char*)(j["frage"] | "");
      }
      vTaskDelay(pdMS_TO_TICKS(700));
      continue;
    }
    char a = kAktion;
    if (a && kJob.length()) {
      kAktion = 0;
      if (a == 'w') { vorlesen(); continue; }
      int code = 0;
      String r = p1Anfrage("POST", "/job/" + kJob + (a == 'j' ? "/ja" : "/nein"), &code);
      JsonDocument j;
      if (!deserializeJson(j, r) && j["ok"]) {
        kSchritt(a == 'j' ? "Uebernommen: GitHub baut, der Giga holt sich die Firmware" : "Verworfen");
        kAenderung = "";
        kMeldung = a == 'j' ? "Uebernommen - Update kommt in ca. 2 min" : "Aenderung verworfen";
      } else kMeldung = j["fehler"].is<const char*>() ? (const char*)j["fehler"] : "Server-Fehler " + String(code);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// ---------- Anzeige ----------
const uint16_t C_CLAUDE = 0xDBAE;                 // Claude-Orange (#D97757)

// Text in Zeilen umbrechen, hoechstens maxZ Zeilen
std::vector<String> umbruch(const String& t, int breite, int maxZ) {
  std::vector<String> z; String zeile, wort;
  auto fertig = [&]() { z.push_back(zeile); zeile = ""; };
  for (int i = 0; i <= (int)t.length(); i++) {
    char c = i < (int)t.length() ? t[i] : ' ';
    if (c == ' ' || c == '\n') {
      String probe = zeile.length() ? zeile + " " + wort : wort;
      if (cv.textWidth(probe) > breite && zeile.length()) { fertig(); zeile = wort; } else zeile = probe;
      wort = "";
      if ((int)z.size() >= maxZ) break;
    } else wort += c;
  }
  if (zeile.length() && (int)z.size() < maxZ) fertig();
  if ((int)z.size() == maxZ && t.length() > 0) { String& l = z.back(); while (l.length() && cv.textWidth(l + "...") > breite) l.remove(l.length() - 1); }
  return z;
}

// Claude-Symbol: Strahlenkranz, dreht sich beim Denken, pulsiert beim Zuhoeren
void claudeSymbol(int x, int y, int r) {
  float drehung = (kz == K_DENKT || kz == K_SENDET) ? animT * 1.6f : 0;
  float puls = kz == K_HOERT ? 0.75f + min(0.5f, kPegel / 80.0f) : kz == K_SPRICHT ? 0.9f + 0.12f * sinf(animT * 14) : 1.0f;
  for (int i = 0; i < 12; i++) {
    float a = drehung + i * PI / 6;
    float l = r * puls * (i % 2 ? 0.78f : 1.0f);
    int x1 = x + cosf(a) * r * 0.25f, y1 = y + sinf(a) * r * 0.25f, x2 = x + cosf(a) * l, y2 = y + sinf(a) * l;
    for (int d = -2; d <= 2; d++) cv.drawLine(x1 + d * sinf(a), y1 - d * cosf(a), x2 + d * sinf(a), y2 - d * cosf(a), C_CLAUDE);
  }
  cv.fillCircle(x, y, r * 0.22f, C_CLAUDE);
}

void zeichneClaude() {
  kopf("Claude");
  claudeSymbol(48, 80, 34);
  const char* st[] = {"Sprechtaste am Giga halten", "Hoert zu ...", "Wird verstanden ...", "Claude arbeitet ...", "Spricht ...", "Fertig", "Fehler"};
  cv.setFont(F12); cv.setTextDatum(top_center); cv.setTextColor(kz == K_FEHLER ? C_RED : C_MUTED);
  cv.drawString(st[kz], 48, 126);
  const int X = 96, BREITE = 216;
  int y = 32;
  cv.setTextDatum(top_left);
  if (kFrage.length()) {
    cv.setFont(F16); cv.setTextColor(C_INK);
    for (auto& z : umbruch("\"" + kFrage + "\"", BREITE, 2)) { cv.drawString(z, X, y); y += 19; }
    y += 4;
  }
  // Zwischenschritte (neueste unten)
  std::vector<String> s;
  xSemaphoreTake(mtx, portMAX_DELAY); s = kSchritte; xSemaphoreGive(mtx);
  bool ende = kz == K_FERTIG || kz == K_SPRICHT || kz == K_FEHLER;
  int platz = ende ? max(0, (150 - y) / 15) : max(0, (210 - y) / 15);
  cv.setFont(F12);
  int ab = max(0, (int)s.size() - platz);
  for (int i = ab; i < (int)s.size(); i++) {
    cv.setTextColor(i == (int)s.size() - 1 && !ende ? C_INK : C_MUTED);
    String t = s[i]; while (t.length() && cv.textWidth(t) > BREITE) t.remove(t.length() - 1);
    cv.drawString(t, X, y); y += 15;
  }
  // Antwort / Aenderung / Meldung unten
  if (ende) {
    int yy = 152;
    if (kAenderung.length()) {
      cv.fillRoundRect(4, yy, 312, 22, 6, C_CLAUDE); cv.setTextColor(C_BG); cv.setFont(F12);
      cv.drawString("Aenderung: " + kAenderung, 10, yy + 5); yy += 26;
    } else if (kMeldung.length()) {
      cv.setTextColor(kz == K_FEHLER ? C_RED : C_OK); cv.setFont(F12);
      for (auto& z : umbruch(kMeldung, 300, 1)) cv.drawString(z, 10, yy + 4);
      yy += 22;
    }
    cv.setFont(F12); cv.setTextColor(C_INK);
    for (auto& z : umbruch(kAntwort, 300, (214 - yy) / 14)) { cv.drawString(z, 10, yy); yy += 14; }
  }
  if (kAenderung.length() && ende) tastenLeiste("Verwerfen", "Nochmal", "Uebernehmen");
  else tastenLeiste("Zurueck", kz == K_SPRICHT ? "Ton aus" : (ende ? "Nochmal" : ""), "");
}

// Tasten auf der Claude-Seite; true = behandelt
bool claudeTasten() {
  if (M5.BtnA.wasClicked()) { if (kAenderung.length() && kz >= K_SPRICHT) kAktion = 'n'; else { seite = P_MENU; sel = 0; } return true; }
  if (M5.BtnB.wasClicked()) { if (kz == K_SPRICHT) kTonStopp = true; else if (kz == K_FERTIG || kz == K_FEHLER) kAktion = 'w'; return true; }
  if (M5.BtnC.wasClicked()) { if (kAenderung.length() && kz >= K_SPRICHT) kAktion = 'j'; return true; }
  return false;
}
