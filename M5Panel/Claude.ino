// ---------- Claude: Sprachsteuerung ----------
// Sprechtaste am Giga halten -> Giga schickt das Mikrofon per MQTT (p1/audio/...) hierher.
// Das M5 streamt die Aufnahme verschluesselt an den Server (jl-parcours, POST /stimme, chunked),
// zeigt dann Claudes Zwischenschritte live, liest die Antwort vor und fragt bei Code-Aenderungen,
// ob sie uebernommen werden sollen (C = ja -> main -> GitHub baut -> Giga holt sich die Firmware).
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Zertifikate.h"

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

void kSchritt(const String& s) { xSemaphoreTake(mtx, portMAX_DELAY); kSchritte.push_back(s); if (kSchritte.size() > 40) kSchritte.erase(kSchritte.begin()); xSemaphoreGive(mtx); }

// ---------- Aufnahme hochladen (laeuft im MQTT-Task, Aufrufe aus den Abonnements) ----------
WiFiClientSecure* upCli = nullptr;
uint32_t upBytes = 0;

void upStart() {
  if (upCli) { upCli->stop(); delete upCli; upCli = nullptr; }
  xSemaphoreTake(mtx, portMAX_DELAY); kSchritte.clear(); xSemaphoreGive(mtx);
  kFrage = ""; kAntwort = ""; kAenderung = ""; kMeldung = ""; kJob = ""; kN = 0; upBytes = 0;
  kz = K_HOERT; kSeit = millis(); kZeigen = true; kTonStopp = true;
  upCli = new WiFiClientSecure();
  upCli->setCACert(ISRG_ROOT_X1_PEM);
  upCli->setTimeout(10);
  if (!upCli->connect(p1Host().c_str(), 443)) { kMeldung = "Server nicht erreichbar"; kz = K_FEHLER; delete upCli; upCli = nullptr; return; }
  upCli->print("POST " + p1Pfad() + "/stimme HTTP/1.1\r\nHost: " + p1Host() + "\r\nX-P1-Token: " P1_TOKEN "\r\n"
               "Content-Type: application/octet-stream\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n");
}

void upDaten(const uint8_t* d, size_t n) {
  if (!upCli || kz != K_HOERT) return;
  char kopf[12]; snprintf(kopf, sizeof(kopf), "%x\r\n", (unsigned)n);
  upCli->print(kopf); upCli->write(d, n); upCli->print("\r\n");
  upBytes += n;
  int mx = 0; for (size_t i = 0; i < n; i++) mx = max(mx, abs((int)d[i] - 128));
  kPegel = mx;
}

void upEnde() {
  if (!upCli) return;
  kz = K_SENDET;
  upCli->print("0\r\n\r\n");
  String antwort; unsigned long t0 = millis();
  while ((upCli->connected() || upCli->available()) && millis() - t0 < 15000) {
    while (upCli->available()) antwort += (char)upCli->read();
    delay(5);
  }
  upCli->stop(); delete upCli; upCli = nullptr;
  int b = antwort.indexOf("\r\n\r\n");
  JsonDocument j;
  if (b < 0 || deserializeJson(j, antwort.substring(b + 4)) || !j["id"].is<const char*>()) {
    String f = b >= 0 ? antwort.substring(b + 4) : "keine Antwort";
    JsonDocument e; if (!deserializeJson(e, f) && e["fehler"].is<const char*>()) f = (const char*)e["fehler"];
    kMeldung = upBytes < 2400 ? "Zu kurz gesprochen - Taste laenger halten" : f; kz = K_FEHLER; return;
  }
  kJob = (const char*)j["id"]; kN = 0; kz = K_DENKT;
  kSchritt(String("Aufnahme ") + String(upBytes / 8000.0, 1) + " s gesendet");
}

// ---------- Abfragen, Vorlesen, Uebernehmen (eigener Task) ----------
WiFiClientSecure kCli;
String p1Anfrage(const char* methode, const String& pfad, int* code = nullptr) {
  HTTPClient h;
  h.setReuse(true); h.setTimeout(15000);
  if (!h.begin(kCli, "https://" + p1Host() + p1Pfad() + pfad)) return "";
  h.addHeader("X-P1-Token", P1_TOKEN);
  int c = strcmp(methode, "POST") == 0 ? h.POST("") : h.GET();
  if (code) *code = c;
  String r = c > 0 ? h.getString() : "";
  h.end();
  return r;
}

void vorlesen() {
  kz = K_SPRICHT; kTonStopp = false;
  HTTPClient h;
  h.setTimeout(30000);
  if (!h.begin(kCli, "https://" + p1Host() + p1Pfad() + "/tts/" + kJob)) { kz = K_FERTIG; return; }
  h.addHeader("X-P1-Token", P1_TOKEN);
  if (h.GET() == 200) {
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
  h.end();
  kz = K_FERTIG;
}

void claudeTask(void*) {
  kCli.setCACert(ISRG_ROOT_X1_PEM);
  for (;;) {
    if (kz == K_DENKT && kJob.length()) {
      String r = p1Anfrage("GET", "/job/" + kJob + "?ab=" + kN);
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
