// Parcours 1 - Bedienpanel auf dem M5Stack Core (3 Tasten), Hochformat 240x320 - Tasten links
//
// Das M5Stack ist MQTT-Broker (PicoMQTT) und meldet sich per UDP-Broadcast im Hotspot.
// Der Giga verbindet sich als Client: kein Internet, kein externer Server.
// Zustand kommt vom Giga (p1/status, p1/einst), Befehle gehen an p1/cmd, p1/set, p1/lampe.
// Lebenszeichen p1/hb alle 400 ms: bleibt es aus, schaltet der Giga die Anlage in den sicheren Zustand.
//
// Tasten:  Start   A = Menue      B = Start/Stopp (lang: NOT-AUS)   C = Kugel freigeben
//          Listen  A = Zurueck    B = Weiter                         C = OK
//          Wert    A = weniger    B = mehr (halten = schnell)        C = Speichern (lang A = Abbrechen)
//
// Bauen: arduino-cli compile -b esp32:esp32:m5stack_core:FlashSize=16M,PartitionScheme=huge_app M5Panel

#include <M5Unified.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <PicoMQTT.h>
#include <ArduinoJson.h>
#include "secrets.h"

// ---------- Farben ----------
const uint16_t C_BG = 0x0841, C_CARD = 0x18E3, C_CARD2 = 0x2945, C_LINE = 0x3186, C_INK = 0xFFFF,
               C_MUTED = 0x8C71, C_FAINT = 0x52AA, C_BLUE = 0x2C3F, C_OK = 0x36AD, C_WARN = 0xFD00,
               C_RED = 0xF9C6, C_TRACK = 0x3C68, C_TRACK_ON = 0x5FEB, C_LIFT = 0xE8A4;
#define F12 &fonts::efontJA_12
#define F16 &fonts::efontJA_16
#define F24 &fonts::efontJA_24
#define FNUM &fonts::FreeSansBold18pt7b

// Bildschirmdrehung: 0 = Hochformat, Inhalt 90 Grad nach links gedreht (Tasten links, A oben).
// Steht das Bild auf dem Kopf, hier 2 eintragen (dann liegen die Tasten rechts, A unten).
const int DREHUNG = 0;

PicoMQTT::Server broker;
WiFiUDP udp;
M5Canvas cv(&M5.Display);

// ---------- Zustand der Anlage (vom Giga) ----------
struct Zustand {
  bool da = false; unsigned long t = 0;
  bool aufzug, leds, stopp, notaus, kugel, sensor, weihnacht, stau;
  int weiche = 1, hebel = 1, rssi = 0, lied = 0, styp = 0;
  uint32_t kugeln = 0, uptime = 0, lauf = 0, flanken = 0;
  char ampel = 'g', lampe = 'a';
  String grund, version, pegel, taster, m5, ip;
} S;

// ---------- Einstellungen (wie im Webinterface) ----------
struct Param { const char* k; const char* n; int mi, ma, st; char typ; int v, d; bool da; };
// typ: 'n' Zahl, 'p' Prozent von 255, 'b' an/aus, 's' Sekunden aus ms, 'g' Grad
Param P[] = {
  {"aufzug_pwm",  "Aufzug Tempo",          0, 255, 5,   'p'}, {"aufzug_umk", "Aufzug umkehren",   0, 1, 1, 'b'},
  {"weiche_pwm",  "Weiche 1 Kraft",        0, 255, 5,   'p'}, {"weiche_ms",  "Weiche 1 Stellweg", 20, 2000, 10, 'n'},
  {"weiche_umk",  "Weiche 1 umkehren",     0, 1, 1,     'b'},
  {"hebel_pwm",   "Weiche 2 Kraft",        0, 255, 5,   'p'}, {"hebel_ms",   "Weiche 2 Stellweg", 20, 2000, 10, 'n'},
  {"hebel_umk",   "Weiche 2 umkehren",     0, 1, 1,     'b'},
  {"stopp_zu",    "Servo Winkel zu",       0, 180, 1,   'g'}, {"stopp_auf",  "Servo Winkel auf",  0, 180, 1, 'g'},
  {"stopp_ms",    "Servo offen fuer",      30, 1500, 10,'n'}, {"stopp_tempo","Servo Tempo",       0, 30, 1, 'n'},
  {"stopp_auto",  "Automatik Freigabe",    0, 1, 1,     'b'}, {"stopp_takt", "Freigabe-Takt",     1000, 30000, 500, 's'},
  {"sensor_an",   "Sensor pruefen",        0, 1, 1,     'b'}, {"sensor_typ", "Shock-Sensor",      0, 1, 1, 'b'},
  {"sensor_empf", "Sensor Empfindl.",      1, 10, 1,    'n'}, {"sensor_sperr","Sensor Sperrzeit", 10, 2000, 10, 'n'},
  {"stau_s",      "Stau-Meldung nach",     0, 300, 5,   'n'}, {"m5_waechter","Stopp bei M5-Verlust",0, 1, 1, 'b'},
  {"led_blink",   "Blinkcode Licht",       0, 1, 1,     'b'}, {"laut",       "Lautstaerke",       0, 100, 5, 'n'},
  {"boost",       "Verstaerkung",          1, 10, 1,    'n'},
};
const int NP = sizeof(P) / sizeof(P[0]);

String wertText(const Param& p, int v) {
  switch (p.typ) {
    case 'b': return v ? "an" : "aus";
    case 'p': return String((v * 100 + 127) / 255) + " %";
    case 's': return String(v / 1000.0, 1) + " s";
    case 'g': return String(v) + "\xC2\xB0";
  }
  String e = String(p.k);
  if (e.endsWith("_ms") || e == "stopp_ms" || e == "sensor_sperr") return String(v) + " ms";
  if (e == "stau_s") return v ? String(v) + " s" : "aus";
  if (e == "laut") return String(v) + " %";
  if (e == "stopp_tempo") return v ? String(v * 100) + " \xC2\xB0/s" : "max";
  if (e == "boost") return v == 1 ? "aus" : String(v) + "x";
  return String(v);
}

// ---------- Statistik ----------
const int MINUTEN = 15, RSSI_N = 150;
uint16_t kugelnProMin[MINUTEN]; int minIdx = 0; unsigned long minStart = 0; uint32_t kugelnMinBasis = 0;
int8_t rssiHist[RSSI_N]; int rssiIdx = 0; unsigned long rssiT = 0;
uint32_t abbrueche = 0, stoerungen = 0; unsigned long verbundenSeit = 0;

// ---------- Oberflaeche ----------
enum Seite { P_HOME, P_MENU, P_STATUS, P_STEUER, P_PARAM, P_EDIT, P_STATS, P_INFO };
Seite seite = P_HOME;
int sel = 0, scroll = 0, statSeite = 0, editWert = 0, menuSel = 0;
unsigned long rueckmeldung = 0; String rueckText;
bool warVerbunden = false; char letzteAmpel = 'g'; unsigned long piepT = 0;
float animT = 0; unsigned long animLast = 0;

const char* MENU_ITEMS[] = {"Status aller Ein-/Ausgaenge", "Manuell steuern", "Betriebsparameter", "Messwerte & Statistik", "Info & Verbindung"};
const int N_MENU = 5;

// Der Giga ist bei seiner minuetlichen GitHub-Abfrage ein paar Sekunden still - das ist kein Abbruch.
// Erst nach 6 s ohne Daten gilt die Verbindung als verloren (der Giga stoppt die Anlage selbst).
const unsigned long VERLOREN_MS = 6000;
bool verbunden() { return S.da && millis() - S.t < VERLOREN_MS; }

// ---------- MQTT ----------
void senden(const char* thema, const String& text) { broker.publish(thema, text); }
void befehl(char c) { senden("p1/cmd", String(c)); }
void meldung(const String& t) { rueckText = t; rueckmeldung = millis(); }

void statusLesen(const char* payload) {
  JsonDocument j;
  if (deserializeJson(j, payload)) return;
  S.aufzug = j["aufzug"]; S.leds = j["leds"]; S.stopp = j["stopp"]; S.notaus = j["notaus"];
  S.kugel = j["kugel"]; S.sensor = j["sensor"]; S.weihnacht = j["weihnacht"]; S.stau = j["stau"];
  S.weiche = j["weiche"] | 1; S.hebel = j["hebel"] | 1; S.rssi = j["rssi"] | 0; S.lied = j["lied"] | 0; S.styp = j["styp"] | 0;
  S.kugeln = j["kugeln"] | 0; S.uptime = j["uptime"] | 0; S.lauf = j["lauf"] | 0; S.flanken = j["flanken"] | 0;
  const char* a = j["ampel"] | "g"; S.ampel = a[0];
  const char* l = j["lampe"] | "a"; S.lampe = l[0];
  S.grund = (const char*)(j["grund"] | ""); S.version = (const char*)(j["version"] | "");
  S.pegel = (const char*)(j["pegel"] | ""); S.taster = (const char*)(j["taster"] | "");
  S.m5 = (const char*)(j["m5"] | ""); S.ip = (const char*)(j["ip"] | "");
  if (!S.da) { kugelnMinBasis = S.kugeln; minStart = millis(); }
  S.da = true; S.t = millis();
}

void einstLesen(const char* payload) {
  JsonDocument j;
  if (deserializeJson(j, payload)) return;
  for (int i = 0; i < NP; i++) {
    JsonArray a = j[P[i].k];
    if (!a.isNull()) { P[i].v = a[0]; P[i].d = a[1]; P[i].da = true; }
  }
}

// ---------- Zeichnen: Grundelemente ----------
// Hochformat 240x320 (Inhalt 90 Grad nach links gedreht). Die drei Tasten liegen dann an der
// linken Kante: A oben, B Mitte, C unten. Ihre Funktion zeigt die Leiste links als Symbol.
const int W = 240, H = 320, RAIL = 30, X0 = RAIL + 6, XR = W - 6, XM = (X0 + XR) / 2;
const int TASTE_Y[3] = {68, 160, 252};     // Hoehe der Tasten A/B/C an der linken Kante

// Symbole fuer die Tastenleiste
void symbol(char s, int x, int y, uint16_t c) {
  switch (s) {
    case 'M': for (int i = -1; i <= 1; i++) cv.fillRoundRect(x - 7, y + i * 5 - 1, 14, 3, 1, c); break;          // Menue
    case 'S': cv.fillTriangle(x - 4, y - 7, x - 4, y + 7, x + 7, y, c); break;                                     // Start
    case 'T': cv.fillRoundRect(x - 6, y - 6, 12, 12, 2, c); break;                                                // Stopp
    case 'K': cv.fillCircle(x, y - 3, 5, c); cv.fillTriangle(x - 5, y + 4, x + 5, y + 4, x, y + 10, c); break;  // Kugel frei
    case 'Z': cv.fillTriangle(x + 5, y - 7, x + 5, y + 7, x - 6, y, c); break;                                     // Zurueck
    case 'W': cv.fillTriangle(x - 7, y - 4, x + 7, y - 4, x, y + 7, c); break;                                     // Weiter
    case 'O': cv.drawLine(x - 7, y, x - 2, y + 5, c); cv.drawLine(x - 2, y + 5, x + 7, y - 5, c);                  // OK / Ausfuehren
              cv.drawLine(x - 7, y + 1, x - 2, y + 6, c); cv.drawLine(x - 2, y + 6, x + 7, y - 4, c); break;
    case '-': cv.fillRect(x - 7, y - 1, 14, 3, c); break;
    case '+': cv.fillRect(x - 7, y - 1, 14, 3, c); cv.fillRect(x - 1, y - 7, 3, 14, c); break;
  }
}
// a/b/c: Symbol je Taste (0 = keine Funktion); rot = Not-Aus-Hinweis bei B (lang halten)
void tastenLeiste(char a, char b, char c, bool bLangNotAus = false) {
  cv.fillRect(0, 0, RAIL, H, C_BG);
  cv.drawFastVLine(RAIL, 0, H, C_LINE);
  char t[3] = {a, b, c};
  for (int i = 0; i < 3; i++) {
    if (!t[i]) continue;
    int y = TASTE_Y[i];
    cv.fillRoundRect(3, y - 24, RAIL - 6, 48, 10, C_CARD2);
    symbol(t[i], RAIL / 2, y, C_INK);
    if (i == 1 && bLangNotAus) cv.fillCircle(RAIL / 2, y + 18, 2, C_RED);
  }
}

void kopf(const char* titel) {
  cv.fillRect(RAIL + 1, 0, W - RAIL - 1, 24, C_BG);
  cv.setFont(F16); cv.setTextDatum(middle_left); cv.setTextColor(C_INK);
  cv.drawString(titel, X0, 12);
  bool ok = verbunden();
  uint16_t f = !ok ? C_RED : S.ampel == 'r' ? C_RED : S.ampel == 'o' ? C_WARN : C_OK;
  int q = !ok ? 0 : S.rssi >= -55 ? 4 : S.rssi >= -67 ? 3 : S.rssi >= -75 ? 2 : 1;
  for (int i = 0; i < 4; i++) cv.fillRect(XR - 22 + i * 6, 17 - i * 3 - 3, 4, i * 3 + 3, i < q ? C_INK : C_LINE);
  cv.fillCircle(XR - 32, 12, 5, f);
  cv.drawFastHLine(RAIL + 1, 24, W - RAIL - 1, C_LINE);
}

// Linie mit Dicke (Strecke)
void bahn(int x0, int y0, int x1, int y1, bool an) {
  uint16_t c = an ? C_TRACK_ON : C_TRACK;
  int w = an ? 3 : 1;
  for (int d = -w / 2; d <= w / 2; d++) {
    if (abs(x1 - x0) > abs(y1 - y0)) cv.drawLine(x0, y0 + d, x1, y1 + d, c);
    else cv.drawLine(x0 + d, y0, x1 + d, y1, c);
  }
}

// ---------- Startseite: Schema der Kugelbahn ----------
// Fahrweg wie ein Liniennetzplan: Aufzug links hoch, oben zur Stoppstelle, Weiche 1 teilt in
// Looping/Vorbei, rechts runter zu Weiche 2, die teilt in Spirale/S-Kurve, unten zurueck zum Aufzug.
void zeichneSchema() {
  const int LX = X0 + 12, TOP = 54, BOT = 200, GX = LX + 46, W1X = LX + 82, RX = XR - 8, W2Y = 118, MX = LX + 24;
  bool l1 = S.weiche != 2, s2 = S.hebel != 2;
  // Aufzug
  cv.fillRoundRect(LX - 7, TOP - 4, 14, BOT - TOP + 8, 4, C_CARD2);
  for (int y = TOP; y <= BOT; y += 10) {
    int yy = BOT - ((y - TOP + (S.aufzug ? (int)(animT * 40) : 0)) % (BOT - TOP + 1));
    cv.fillRect(LX - 3, yy, 6, 3, S.aufzug ? C_LIFT : C_FAINT);
  }
  // oben zur Stoppstelle und weiter zur Weiche 1
  bahn(LX, TOP, W1X, TOP, true);
  cv.fillRect(GX - 2, TOP - 16, 4, 6, C_MUTED);
  if (S.stopp) cv.fillRect(GX - 2, TOP - 22, 14, 4, C_OK);
  else cv.fillRect(GX - 2, TOP - 12, 4, 16, C_RED);
  cv.fillCircle(GX - 13, TOP + 10, 4, S.kugel ? C_OK : C_FAINT);       // Sensor
  if (S.kugel) cv.drawCircle(GX - 13, TOP + 10, 7, C_OK);
  // Weiche 1: oben Looping, unten Vorbei
  bahn(W1X, TOP, RX - 16, TOP, l1);
  cv.drawCircle(W1X + 44, TOP - 11, 10, l1 ? C_TRACK_ON : C_TRACK); if (l1) cv.drawCircle(W1X + 44, TOP - 11, 9, C_TRACK_ON);
  bahn(W1X, TOP, W1X + 14, TOP + 26, !l1); bahn(W1X + 14, TOP + 26, RX - 16, TOP + 26, !l1);
  bahn(RX - 16, TOP, RX, TOP + 13, l1); bahn(RX - 16, TOP + 26, RX, TOP + 13, !l1);
  cv.fillCircle(W1X, TOP, 5, C_BLUE);
  // rechts runter zu Weiche 2
  bahn(RX, TOP + 13, RX, W2Y, true);
  cv.fillCircle(RX, W2Y, 5, C_BLUE);
  // Weiche 2: oben Spirale (Zickzack), unten S-Kurve (Welle), nach links zurueck
  bahn(RX, W2Y, RX - 14, W2Y + 12, s2); bahn(RX, W2Y, RX - 14, W2Y + 44, !s2);
  uint16_t cs = s2 ? C_TRACK_ON : C_TRACK;
  for (int x = RX - 14; x > MX + 14; x -= 16) {
    cv.drawLine(x, W2Y + 12, x - 8, W2Y + 3, cs); cv.drawLine(x - 8, W2Y + 3, x - 16, W2Y + 12, cs);
    if (s2) { cv.drawLine(x, W2Y + 13, x - 8, W2Y + 4, cs); cv.drawLine(x - 8, W2Y + 4, x - 16, W2Y + 13, cs); }
  }
  for (int x = RX - 14; x > MX + 4; x -= 3) {
    int y = W2Y + 44 + (int)(6 * sin((x - RX) / 14.0)), y2 = W2Y + 44 + (int)(6 * sin((x - 3 - RX) / 14.0));
    cv.drawLine(x, y, x - 3, y2, s2 ? C_TRACK : C_TRACK_ON);
    if (!s2) cv.drawLine(x, y + 1, x - 3, y2 + 1, C_TRACK_ON);
  }
  bahn(MX + 14, W2Y + 12, MX, BOT, s2); bahn(MX + 4, W2Y + 44, MX, BOT, !s2);
  bahn(MX, BOT, LX, BOT, true);
  // Beschriftung
  cv.setFont(F12); cv.setTextDatum(top_left);
  cv.setTextColor(l1 ? C_INK : C_FAINT); cv.drawString("Looping", W1X + 12, TOP + 5);
  cv.setTextColor(!l1 ? C_INK : C_FAINT); cv.drawString("Vorbei", W1X + 22, TOP + 30);
  cv.setTextColor(s2 ? C_INK : C_FAINT); cv.drawString("Spirale", MX + 30, W2Y + 16);
  cv.setTextColor(!s2 ? C_INK : C_FAINT); cv.drawString("S-Kurve", MX + 30, W2Y + 54);
  cv.setTextColor(C_MUTED); cv.drawString("Aufzug", LX + 10, BOT - 16); cv.drawString("Stopp", GX + 6, TOP - 30 < 28 ? 28 : TOP - 30);
  // fahrende Kugel (nur Anzeige) auf dem aktiven Weg
  if (S.aufzug) {
    float u = fmod(animT * 0.12f, 1.0f); int bx, by;
    if (u < 0.25f) { bx = LX; by = BOT - (int)((BOT - TOP) * u / 0.25f); }
    else if (u < 0.45f) { bx = LX + (int)((RX - LX) * (u - 0.25f) / 0.2f); by = l1 || bx < W1X + 14 ? TOP : TOP + 26; }
    else if (u < 0.55f) { bx = RX; by = TOP + 13 + (int)((W2Y - TOP - 13) * (u - 0.45f) / 0.1f); }
    else if (u < 0.85f) { bx = RX - (int)((RX - MX) * (u - 0.55f) / 0.3f); by = s2 ? W2Y + 9 : W2Y + 44; }
    else { bx = MX - (int)((MX - LX) * (u - 0.85f) / 0.15f); by = BOT; }
    cv.fillCircle(bx, by, 4, C_INK);
  }
}

void zeichneHome() {
  kopf("Parcours 1");
  zeichneSchema();
  bool ok = verbunden();
  // Zustandskarte
  uint16_t bg = !ok || S.ampel == 'r' ? C_RED : S.ampel == 'o' ? C_WARN : C_CARD;
  cv.fillRoundRect(X0 - 2, 214, XR - X0 + 4, 56, 10, bg);
  cv.setTextDatum(top_left); cv.setTextColor(C_INK); cv.setFont(F16);
  String z = !ok ? "Keine Verbindung" : S.notaus ? "NOT-AUS" : S.stau ? "Kugelstau" : S.ampel == 'r' ? "STOERUNG" : S.aufzug ? "In Betrieb" : "Bereit";
  cv.drawString(z, X0 + 6, 222);
  cv.setFont(F12); cv.setTextColor(ok && S.ampel == 'g' ? C_MUTED : C_INK);
  String u = !ok ? "sicherer Zustand" : S.ampel != 'g' ? S.grund : String(S.weiche == 2 ? "Vorbei" : "Looping") + " / " + (S.hebel == 2 ? "S-Kurve" : "Spirale");
  if (u.length() > 22) u = u.substring(0, 21) + "...";
  cv.drawString(u, X0 + 6, 246);
  cv.setTextDatum(top_right); cv.setFont(FNUM); cv.setTextColor(C_INK);
  cv.drawString(String(S.kugeln), XR - 6, 220);
  cv.setFont(F12); cv.setTextColor(ok && S.ampel == 'g' ? C_MUTED : C_INK); cv.drawString("Kugeln", XR - 6, 252);
  // zwei Infozeilen
  cv.setFont(F12); cv.setTextDatum(top_left);
  cv.setTextColor(S.kugel ? C_OK : C_MUTED);
  cv.drawString(String("Sensor: ") + (S.styp ? (S.kugel ? "Kugel wartet" : "warte auf Stoss") : (S.kugel ? "Kugel da" : "leer")), X0, 280);
  cv.setTextColor(C_MUTED);
  cv.drawString(String("Licht ") + (S.leds ? "an" : "aus") + "  \xC2\xB7  Stopp " + (S.stopp ? "offen" : "zu"), X0, 298);
  tastenLeiste('M', S.aufzug ? 'T' : 'S', 'K', true);
}

// Liste mit Auswahl (Menue, Steuerung, Parameter)
void liste(const char* titel, int n, std::function<void(int, String&, String&, uint16_t&)> zeile) {
  kopf(titel);
  const int RH = 32, Y0 = 30, SICHT = 9;
  if (sel < scroll) scroll = sel;
  if (sel >= scroll + SICHT) scroll = sel - SICHT + 1;
  for (int i = scroll; i < min(n, scroll + SICHT); i++) {
    int y = Y0 + (i - scroll) * RH;
    String a, b; uint16_t fb = C_MUTED; zeile(i, a, b, fb);
    bool s = i == sel;
    cv.fillRoundRect(X0 - 2, y, XR - X0 + 4, RH - 4, 8, s ? C_BLUE : C_CARD);
    cv.setFont(F12); cv.setTextDatum(middle_left); cv.setTextColor(C_INK); cv.drawString(a, X0 + 6, y + 14);
    cv.setTextDatum(middle_right); cv.setTextColor(s ? C_INK : fb); cv.drawString(b, XR - 6, y + 14);
  }
  if (n > SICHT) {
    int h = 286 * SICHT / n, y = 30 + (286 - h) * scroll / max(1, n - SICHT);
    cv.fillRect(W - 3, y, 3, h, C_FAINT);
  }
}

void zeichneMenu() {
  liste("Menue", N_MENU, [](int i, String& a, String& b, uint16_t& f) { a = MENU_ITEMS[i]; b = ">"; });
  tastenLeiste('Z', 'W', 'O');
}

// ---------- Status aller Ein-/Ausgaenge ----------
// zweizeilig: Bezeichnung klein oben, Wert rechts
void zeile2(int y, const String& a, const String& b, uint16_t f = C_INK) {
  cv.setFont(F12); cv.setTextDatum(middle_left); cv.setTextColor(C_MUTED); cv.drawString(a, X0, y);
  cv.setFont(F16); cv.setTextDatum(middle_right); cv.setTextColor(f); cv.drawString(b, XR, y);
  cv.drawFastHLine(X0, y + 14, XR - X0, C_CARD);
}
const int STATUS_SEITEN = 3;
void zeichneStatus() {
  const char* t[] = {"Ausgaenge 1/3", "Eingaenge 2/3", "System 3/3"};
  kopf(t[statSeite]);
  int y = 44, d = 32;
  if (statSeite == 0) {
    zeile2(y, "Aufzug (M2)", S.aufzug ? "laeuft" : "aus", S.aufzug ? C_OK : C_INK); y += d;
    zeile2(y, "Weiche 1 (M3)", S.weiche == 2 ? "Vorbei" : "Looping"); y += d;
    zeile2(y, "Weiche 2 (M4)", S.hebel == 2 ? "S-Kurve" : "Spirale"); y += d;
    zeile2(y, "Stoppstelle (Servo D2)", S.stopp ? "offen" : "zu", S.stopp ? C_OK : C_INK); y += d;
    zeile2(y, "Licht (M1)", S.leds ? "an" : "aus"); y += d;
    zeile2(y, "Lautsprecher", S.weihnacht ? "spielt" : "still"); y += d;
    const char* lf = S.lampe == 'a' ? "Auto" : S.lampe == 'g' ? "Gruen" : S.lampe == 'o' ? "Orange" : S.lampe == 'r' ? "Rot" : S.lampe == 'b' ? "Blau" : S.lampe == 'w' ? "Weiss" : "Aus";
    zeile2(y, "Statuslampe (A0-A2)", lf);
  } else if (statSeite == 1) {
    zeile2(y, S.styp ? "Shock-Sensor (A6)" : "Reflex-Sensor (A6)", S.kugel ? "Kugel" : "leer", S.kugel ? C_OK : C_INK); y += d;
    zeile2(y, "Pegel", S.pegel); y += d;
    zeile2(y, "Kontakte gesamt", String(S.flanken)); y += d;
    zeile2(y, "Sensorpruefung", S.sensor ? "aktiv" : "aus"); y += d;
    String tl = S.taster.length() == 3 ? S.taster : "000";
    zeile2(y, "Taster links", tl[0] == '1' ? "gedrueckt" : "frei"); y += d;
    zeile2(y, "Taster Mitte", tl[1] == '1' ? "gedrueckt" : "frei"); y += d;
    zeile2(y, "Taster rechts", tl[2] == '1' ? "gedrueckt" : "frei"); y += d;
    zeile2(y, "Kugeln gezaehlt", String(S.kugeln));
  } else {
    zeile2(y, "Anlage", S.ampel == 'g' ? "alles ok" : S.ampel == 'o' ? "kein Internet" : "Stoerung", S.ampel == 'g' ? C_OK : S.ampel == 'o' ? C_WARN : C_RED); y += d;
    String g = S.grund.length() ? S.grund : "-"; if (g.length() > 30) g = g.substring(0, 29) + "...";
    cv.setFont(F12); cv.setTextDatum(middle_left); cv.setTextColor(C_MUTED); cv.drawString(g, X0, y - 8); y += d - 8;
    zeile2(y, "WLAN Giga", String(S.rssi) + " dBm"); y += d;
    zeile2(y, "WLAN M5Stack", String(WiFi.RSSI()) + " dBm"); y += d;
    zeile2(y, "Firmware Giga", S.version); y += d;
    uint32_t u = S.uptime; char b[24]; snprintf(b, sizeof(b), "%lu:%02lu:%02lu", (unsigned long)(u / 3600), (unsigned long)(u / 60 % 60), (unsigned long)(u % 60));
    zeile2(y, "Giga laeuft seit", b);
  }
  tastenLeiste('Z', 'W', 0);
}

// ---------- Manuell steuern ----------
const char* STEUER_TXT[] = {"Aufzug", "Kugel freigeben", "Weiche 1", "Weiche 2", "Servo-Test", "Licht", "Statuslampe", "Weihnachtsmodus", "NOT-AUS"};
const int N_STEUER = 9;
const char LAMPEN[] = {'a', 'g', 'o', 'r', 'b', 'w', '0'};
void zeichneSteuer() {
  liste("Manuell steuern", N_STEUER, [](int i, String& a, String& b, uint16_t& f) {
    a = STEUER_TXT[i];
    switch (i) {
      case 0: b = S.aufzug ? "laeuft" : "aus"; f = S.aufzug ? C_OK : C_MUTED; break;
      case 1: b = S.stopp ? "offen" : "zu"; break;
      case 2: b = S.weiche == 2 ? "Vorbei" : "Looping"; break;
      case 3: b = S.hebel == 2 ? "S-Kurve" : "Spirale"; break;
      case 4: b = "auf+zu"; break;
      case 5: b = S.leds ? "an" : "aus"; break;
      case 6: { const char* n[] = {"Auto", "Gruen", "Orange", "Rot", "Blau", "Weiss", "Aus"}; int k = 0; for (int j = 0; j < 7; j++) if (LAMPEN[j] == S.lampe) k = j; b = n[k]; break; }
      case 7: b = !S.lied ? "kein Lied" : S.weihnacht ? "an" : "aus"; break;
      case 8: b = S.notaus ? "aktiv" : ""; f = C_RED; break;
    }
  });
  tastenLeiste('Z', 'W', 'O');
}
void steuerAusfuehren(int i) {
  const char c[] = {'a', 's', 'w', 'h', 'v', 'l', 0, 'j', 'x'};
  if (i == 6) { int k = 0; for (int j = 0; j < 7; j++) if (LAMPEN[j] == S.lampe) k = j; senden("p1/lampe", String(LAMPEN[(k + 1) % 7])); meldung("Lampe gewechselt"); return; }
  befehl(c[i]);
  meldung(i == 8 ? "NOT-AUS" : String(STEUER_TXT[i]));
}

// ---------- Parameter ----------
void zeichneParam() {
  liste("Parameter", NP, [](int i, String& a, String& b, uint16_t& f) { a = P[i].n; b = P[i].da ? wertText(P[i], P[i].v) : "?"; if (P[i].da && P[i].v != P[i].d) f = C_BLUE; });
  tastenLeiste('Z', 'W', 'O');
}
void zeichneEdit() {
  Param& p = P[sel];
  kopf(p.n);
  cv.setTextDatum(middle_center); cv.setTextColor(C_INK); cv.setFont(F24);
  cv.drawString(wertText(p, editWert), XM, 110);
  int bw = XR - X0, x = X0, y = 160;
  cv.fillRoundRect(x, y, bw, 10, 5, C_CARD2);
  cv.fillRoundRect(x, y, max(10, bw * (editWert - p.mi) / max(1, p.ma - p.mi)), 10, 5, C_BLUE);
  cv.setFont(F12); cv.setTextColor(C_MUTED);
  cv.setTextDatum(top_left); cv.drawString(wertText(p, p.mi), x, y + 16);
  cv.setTextDatum(top_right); cv.drawString(wertText(p, p.ma), x + bw, y + 16);
  cv.setTextDatum(top_center);
  cv.drawString("Startwert: " + wertText(p, p.d), XM, 210);
  cv.drawString("A lang halten = abbrechen", XM, 230);
  tastenLeiste('-', '+', 'O');
}

// ---------- Statistik ----------
const int STAT_SEITEN = 3;
void zeichneStats() {
  const char* t[] = {"Kugeln/min 1/3", "WLAN 5 min 2/3", "Uebersicht 3/3"};
  kopf(t[statSeite]);
  if (statSeite == 0) {
    int mx = 1; for (int i = 0; i < MINUTEN; i++) mx = max(mx, (int)kugelnProMin[i]);
    uint32_t jetzt = S.kugeln - kugelnMinBasis; mx = max(mx, (int)jetzt);
    const int GX = X0 + 18, Y1 = 270, GH = 220, BW = (XR - GX) / (MINUTEN + 1);
    cv.drawFastHLine(GX, Y1, XR - GX, C_LINE);
    cv.setFont(F12); cv.setTextColor(C_MUTED); cv.setTextDatum(middle_right); cv.drawString(String(mx), GX - 3, Y1 - GH); cv.drawString("0", GX - 3, Y1);
    for (int i = 0; i < MINUTEN; i++) {
      int k = (minIdx + 1 + i) % MINUTEN, v = kugelnProMin[k], h = GH * v / mx;
      cv.fillRect(GX + 2 + i * BW, Y1 - h, BW - 2, h, C_TRACK_ON);
    }
    int h = GH * (int)jetzt / mx; cv.fillRect(GX + 2 + MINUTEN * BW, Y1 - h, BW - 2, h, C_BLUE);
    cv.setTextDatum(top_left); cv.drawString("-15 min", GX, Y1 + 6); cv.setTextDatum(top_right); cv.drawString("jetzt", XR, Y1 + 6);
  } else if (statSeite == 1) {
    const int GX = X0 + 22, GY = 44, GW = XR - GX, GH = 220;
    cv.setFont(F12); cv.setTextColor(C_MUTED); cv.setTextDatum(middle_right);
    for (int db = -30; db >= -90; db -= 20) { int y = GY + GH * (-30 - db) / 60; cv.drawFastHLine(GX, y, GW, C_LINE); cv.drawString(String(db), GX - 3, y); }
    int px = -1, py = 0;
    for (int i = 0; i < RSSI_N; i++) {
      int v = rssiHist[(rssiIdx + i) % RSSI_N]; if (v == 0) { px = -1; continue; }
      int x = GX + GW * i / (RSSI_N - 1), y = GY + GH * constrain(-30 - v, 0, 60) / 60;
      if (px >= 0) { cv.drawLine(px, py, x, y, C_TRACK_ON); cv.drawLine(px, py + 1, x, y + 1, C_TRACK_ON); }
      px = x; py = y;
    }
    cv.setTextDatum(top_left); cv.drawString("Giga in dBm, hoeher = besser", X0, GY + GH + 10);
  } else {
    int y = 44, d = 34; uint32_t up = max<uint32_t>(1, S.uptime);
    char b[32];
    zeile2(y, "Kugeln gesamt", String(S.kugeln)); y += d;
    snprintf(b, sizeof(b), "%.1f", S.lauf > 0 ? S.kugeln * 60.0 / S.lauf : 0.0); zeile2(y, "Kugeln/min im Betrieb", b); y += d;
    snprintf(b, sizeof(b), "%lu:%02lu", (unsigned long)(S.lauf / 60), (unsigned long)(S.lauf % 60)); zeile2(y, "Aufzug gelaufen (min)", b); y += d;
    snprintf(b, sizeof(b), "%d %%", (int)(S.lauf * 100 / up)); zeile2(y, "Auslastung", b); y += d;
    zeile2(y, "Stoerungen", String(stoerungen), stoerungen ? C_WARN : C_INK); y += d;
    zeile2(y, "Verbindungsabbrueche", String(abbrueche), abbrueche ? C_WARN : C_INK);
  }
  tastenLeiste('Z', 'W', 0);
}

void zeichneInfo() {
  kopf("Info");
  int y = 44, d = 34;
  zeile2(y, "M5Stack (Broker)", WiFi.localIP().toString()); y += d;
  zeile2(y, "WLAN", WiFi.SSID()); y += d;
  zeile2(y, "Giga", S.ip.length() ? S.ip : "-"); y += d;
  zeile2(y, "Giga sieht M5", S.m5.length() ? S.m5 : "-"); y += d;
  zeile2(y, "Verbunden seit", verbunden() ? String((millis() - verbundenSeit) / 1000) + " s" : "-"); y += d;
  cv.setFont(F12); cv.setTextDatum(middle_left); cv.setTextColor(C_MUTED);
  cv.drawString("Web: http://" + (S.ip.length() ? S.ip : String("...")), X0, y);
  tastenLeiste('Z', 0, 0);
}

// Grosse Warnung bei Verbindungsverlust
void zeichneVerloren() {
  cv.fillScreen(C_RED);
  cv.setTextColor(C_INK); cv.setTextDatum(middle_center);
  cv.setFont(F24);
  if (S.da) { cv.drawString("Verbindung", XM, 80); cv.drawString("verloren", XM, 108); }
  else cv.drawString("Suche Anlage", XM, 94);
  cv.setFont(F16);
  if (S.da) {
    cv.drawString("Anlage ist im", XM, 160); cv.drawString("sicheren Zustand:", XM, 180);
    cv.drawString("Motoren aus,", XM, 204); cv.drawString("Stoppstelle zu.", XM, 224);
    cv.setFont(F12); cv.drawString("seit " + String((millis() - S.t) / 1000) + " s keine Daten", XM, 260);
  } else {
    cv.drawString(WiFi.status() == WL_CONNECTED ? "WLAN ok" : "Verbinde WLAN ...", XM, 150);
    cv.setFont(F12);
    cv.drawString(WiFi.status() == WL_CONNECTED ? "warte auf den Giga" : String(WLAN_SSID), XM, 176);
    if (WiFi.status() == WL_CONNECTED) cv.drawString("Broker " + WiFi.localIP().toString(), XM, 196);
  }
  tastenLeiste('M', 0, 0);
}

// ---------- Tasten ----------
void tasten() {
  M5.update();
  auto& A = M5.BtnA; auto& B = M5.BtnB; auto& C = M5.BtnC;
  static unsigned long wiederholT = 0;
  // Startseite (bzw. Warnseite)
  if (seite == P_HOME) {
    if (A.wasClicked()) { seite = P_MENU; sel = 0; scroll = 0; }
    if (B.wasHold()) { befehl('x'); meldung("NOT-AUS"); M5.Speaker.tone(1200, 300); }
    else if (B.wasClicked()) { befehl('a'); meldung(S.aufzug ? "Stopp" : "Start"); }
    if (C.wasClicked()) { befehl('s'); meldung("Kugel freigegeben"); }
    return;
  }
  if (seite == P_EDIT) {
    Param& p = P[sel];
    if (A.wasHold()) { seite = P_PARAM; return; }
    int dir = 0;
    if (A.wasClicked()) dir = -1;
    if (B.wasClicked()) dir = 1;
    if ((A.isHolding() || B.isHolding()) && millis() - wiederholT > 70) { wiederholT = millis(); dir = A.isHolding() ? -1 : 1; }
    if (p.typ == 'b' && dir) editWert = dir > 0 ? 1 : 0;
    else editWert = constrain(editWert + dir * p.st, p.mi, p.ma);
    if (C.wasClicked()) { senden("p1/set", String(p.k) + "=" + editWert); meldung("Gespeichert"); seite = P_PARAM; }
    return;
  }
  if (A.wasClicked()) {                              // Zurueck
    if (seite == P_MENU) seite = P_HOME;
    else { seite = P_MENU; sel = menuSel; scroll = 0; }
    statSeite = 0; return;
  }
  int n = seite == P_MENU ? N_MENU : seite == P_STEUER ? N_STEUER : seite == P_PARAM ? NP : 0;
  if (B.wasClicked()) {
    if (n) sel = (sel + 1) % n;
    if (seite == P_STATUS) statSeite = (statSeite + 1) % STATUS_SEITEN;
    if (seite == P_STATS) statSeite = (statSeite + 1) % STAT_SEITEN;
  }
  if (C.wasClicked()) {
    if (seite == P_MENU) { menuSel = sel; const Seite z[] = {P_STATUS, P_STEUER, P_PARAM, P_STATS, P_INFO}; seite = z[sel]; sel = 0; scroll = 0; statSeite = 0; if (seite == P_PARAM) senden("p1/get", "1"); }
    else if (seite == P_STEUER) steuerAusfuehren(sel);
    else if (seite == P_PARAM) {
      Param& p = P[sel];
      if (!p.da) { senden("p1/get", "1"); meldung("Werte werden geladen"); }
      else if (p.typ == 'b') { senden("p1/set", String(p.k) + "=" + (p.v ? 0 : 1)); meldung(String(p.n) + (p.v ? " aus" : " an")); }
      else { editWert = p.v; seite = P_EDIT; }
    }
  }
}

// ---------- Setup / Loop ----------
void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setRotation(DREHUNG);
  M5.Speaker.setVolume(140);
  cv.setColorDepth(16);
  if (!cv.createSprite(W, H)) { cv.setColorDepth(8); cv.createSprite(W, H); }
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  broker.subscribe("p1/status", [](const char* topic, const char* payload) { statusLesen(payload); });
  broker.subscribe("p1/einst", [](const char* topic, const char* payload) { einstLesen(payload); });
  broker.begin();
  udp.begin(47901);
}

void loop() {
  broker.loop();
  tasten();
  unsigned long now = millis();
  static unsigned long hbT = 0, suchT = 0, zeichenT = 0;
  if (WiFi.status() == WL_CONNECTED) {
    if (now - hbT >= 400) { hbT = now; senden("p1/hb", "1"); }
    if (now - suchT >= 1000) { suchT = now; udp.beginPacket(WiFi.broadcastIP(), 47900); udp.print("P1BROKER"); udp.endPacket(); }
  }
  // Verbindung / Stoerungen melden
  bool ok = verbunden();
  if (ok && !warVerbunden) { verbundenSeit = now; senden("p1/get", "1"); }
  if (!ok && warVerbunden) { abbrueche++; for (int i = 0; i < 3; i++) { M5.Speaker.tone(880, 120); delay(180); } piepT = now; }
  warVerbunden = ok;
  if (!ok && S.da && now - piepT > 60000) { piepT = now; M5.Speaker.tone(880, 120); }   // Erinnerung nur 1x pro Minute
  if (ok && S.ampel == 'r' && letzteAmpel != 'r') { stoerungen++; M5.Speaker.tone(1760, 150); delay(170); M5.Speaker.tone(1760, 150); }
  if (ok) letzteAmpel = S.ampel;
  // Statistik
  if (S.da && now - minStart >= 60000) {
    minStart = now; minIdx = (minIdx + 1) % MINUTEN;
    kugelnProMin[minIdx] = S.kugeln - kugelnMinBasis; kugelnMinBasis = S.kugeln;
  }
  if (now - rssiT >= 2000) { rssiT = now; rssiIdx = (rssiIdx + 1) % RSSI_N; rssiHist[(rssiIdx + RSSI_N - 1) % RSSI_N] = ok ? S.rssi : 0; }
  // Zeichnen (ca. 25 Bilder pro Sekunde)
  if (now - zeichenT < 40) return;
  float dt = (now - animLast) / 1000.0f; animLast = now; animT += dt; zeichenT = now;
  cv.fillScreen(C_BG);
  if (!ok && (seite == P_HOME || !S.da)) zeichneVerloren();
  else switch (seite) {
    case P_HOME: zeichneHome(); break;
    case P_MENU: zeichneMenu(); break;
    case P_STATUS: zeichneStatus(); break;
    case P_STEUER: zeichneSteuer(); break;
    case P_PARAM: zeichneParam(); break;
    case P_EDIT: zeichneEdit(); break;
    case P_STATS: zeichneStats(); break;
    case P_INFO: zeichneInfo(); break;
  }
  if (!ok && seite != P_HOME && S.da) {               // auch in Menues: rote Leiste
    cv.fillRect(RAIL + 1, 0, W - RAIL - 1, 24, C_RED); cv.setFont(F12); cv.setTextColor(C_INK); cv.setTextDatum(middle_center);
    cv.drawString("Verbindung verloren", XM, 12);
  } else if (ok && S.ampel == 'r' && seite != P_HOME) {
    cv.fillRect(RAIL + 1, 0, W - RAIL - 1, 24, C_RED); cv.setFont(F12); cv.setTextColor(C_INK); cv.setTextDatum(middle_center);
    String g = S.grund; if (g.length() > 26) g = g.substring(0, 25) + "..."; cv.drawString(g, XM, 12);
  }
  if (now - rueckmeldung < 1200) {                   // kurze Rueckmeldung nach einer Aktion
    cv.setFont(F16); int w = min(W - RAIL - 8, (int)cv.textWidth(rueckText) + 28);
    cv.fillRoundRect(XM - w / 2, 140, w, 34, 17, C_INK); cv.setTextColor(C_BG); cv.setTextDatum(middle_center); cv.drawString(rueckText, XM, 157);
  }
  cv.pushSprite(0, 0);
}
