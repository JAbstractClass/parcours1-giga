// ---------- WLAN, Weboberflaeche, OTA-Update ----------
//
// WLAN laut arduino_secrets.h: vorhandenes Netz (z. B. Hotspot) oder eigenes Netz "Parcours1"
// (http://192.168.3.1). Klappt das vorhandene Netz nicht, startet automatisch das eigene.
// Die Adresse steht beim Start im seriellen Monitor und mit '?'.
// Seiten:
//   /            Bedienoberflaeche (mehrere Geraete gleichzeitig moeglich)
//   /status      Zustand als JSON
//   /cmd?c=X     Befehl ausfuehren (gleiche Buchstaben wie im seriellen Monitor)
//   /update?url= Firmware per WLAN laden (.ota-Datei, z. B. von ./ota.sh bereitgestellt)

#include <WiFi.h>
#include <WiFiUdp.h>
#include <Arduino_Portenta_OTA.h>
#include "arduino_secrets.h"
#include "Zertifikate.h"
#include "utility/https_request.h"

// version.h wird von GitHub Actions (bzw. ./flash.sh) erzeugt: FW_COMMIT + OTA_BASIS
#if __has_include("version.h")
#include "version.h"
#endif
#ifndef FW_COMMIT
#define FW_COMMIT "lokal"     // lokal gebaut -> kein automatisches Update
#endif
#ifndef OTA_BASIS
#define OTA_BASIS "https://jabstractclass.github.io/parcours1-giga"
#endif

const char FW_VERSION[] = FW_COMMIT;
const char FW_DATUM[]   = __DATE__ " " __TIME__;
const int  OTA_PORT = 8000;   // Port, auf dem ./ota.sh die .ota-Datei anbietet
const unsigned long UPDATE_PRUEF_MS = 60000;   // so oft auf GitHub nach neuer Version schauen

WiFiServer server(80);
bool wlanOk = false;
String otaUrl = "";           // gesetzt -> Update wird im naechsten loop ausgefuehrt
String otaMeldung = "";
String neuesteVersion = "";   // zuletzt auf GitHub gesehener Commit
unsigned long letztePruefung = 0;
bool erstePruefung = true;
String fehlVersion = "";      // Auto-Update dieser Version ist fehlgeschlagen -> nicht endlos wiederholen

// Weboberflaeche: Quelle ist dashboard.html, Dashboard.h wird daraus erzeugt (tools/html2h.py)
#include "Dashboard.h"

// ---------- Uhrzeit (fuer HTTPS noetig: sonst gelten Zertifikate als "noch nicht gueltig") ----------
bool zeitGesetzt = false;

bool ntpZeit() {
  WiFiUDP udp;
  if (!udp.begin(2390)) return false;
  uint8_t pkt[48] = {0};
  pkt[0] = 0b11100011;               // LI, Version 4, Client
  bool ok = false;
  for (int versuch = 0; versuch < 3 && !ok; versuch++) {
    udp.beginPacket("pool.ntp.org", 123);
    udp.write(pkt, sizeof(pkt));
    udp.endPacket();
    unsigned long t0 = millis();
    while (millis() - t0 < 1500) {
      if (udp.parsePacket() >= 48) {
        uint8_t r[48];
        udp.read(r, 48);
        unsigned long sek = ((unsigned long)r[40] << 24) | ((unsigned long)r[41] << 16) | ((unsigned long)r[42] << 8) | r[43];
        set_time(sek - 2208988800UL);  // NTP (1900) -> Unix (1970)
        ok = true;
        break;
      }
      delay(10);
    }
  }
  udp.stop();
  return ok;
}

void zeitSicherstellen() {
  if (zeitGesetzt) return;
  zeitGesetzt = ntpZeit();
  Serial.print("Uhrzeit: ");
  if (zeitGesetzt) { time_t t = time(NULL); Serial.print(ctime(&t)); }
  else Serial.println("NTP fehlgeschlagen");
}

// ---------- HTTPS mit eingebautem Zertifikat ----------
// Die Zertifikatsliste auf dem Board (/wlan/cacert.pem) funktioniert fuer GitHub nicht
// (Fehler -3011). Deshalb wird ISRG Root X1 direkt mitgegeben.
// Rueckgabe: HTTP-Statuscode (200 = ok) oder negativer Fehlercode.
int httpsAbruf(const String& url, mbed::Callback<void(const char*, uint32_t)> cbk) {
  zeitSicherstellen();
  HttpsRequest* req = new HttpsRequest(WiFi.getNetwork(), ISRG_ROOT_X1_PEM, HTTP_GET, url.c_str(), cbk);
  HttpResponse* rsp = req->send(NULL, 0);
  int code;
  if (!rsp) {
    code = req->get_error();
    if (code >= 0) code = -1;
  } else {
    unsigned long t0 = millis();
    while (!rsp->is_message_complete() && millis() - t0 < 60000) delay(10);
    code = rsp->is_message_complete() ? (int)rsp->get_status_code() : -2;
  }
  delete req;
  return code;
}

// ---------- WLAN ----------
void wlanScan() {
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    Serial.print("  WLAN: \""); Serial.print(WiFi.SSID(i)); Serial.print("\"  ");
    Serial.print(WiFi.RSSI(i)); Serial.println(" dBm");
  }
  if (n <= 0) Serial.println("  (keine Netze gefunden)");
}

void webSetup() {
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WLAN-Modul antwortet nicht (WLAN-Firmware fehlt?)");
    return;
  }
  int st = WL_IDLE_STATUS;
  if (!WLAN_EIGENES_NETZ && strlen(WLAN_SSID) > 0) {
    Serial.print("Verbinde mit WLAN '"); Serial.print(WLAN_SSID); Serial.println("' ...");
    for (int i = 0; i < 3 && st != WL_CONNECTED; i++) st = WiFi.begin(WLAN_SSID, WLAN_PASS);
    if (st != WL_CONNECTED) {
      Serial.println("WLAN-Verbindung fehlgeschlagen -> eigenes WLAN. Sichtbare Netze:");
      wlanScan();
    }
  }
  if (st != WL_CONNECTED) {
    st = WiFi.beginAP(AP_SSID, AP_PASS);
    if (st != WL_AP_LISTENING) {
      Serial.print("Eigenes WLAN konnte nicht gestartet werden, Status "); Serial.println(st);
      return;
    }
    Serial.print("Eigenes WLAN '"); Serial.print(AP_SSID); Serial.println("' aktiv");
  }
  delay(500);
  server.begin();
  wlanOk = true;
  Serial.print("Weboberflaeche: http://"); Serial.println(WiFi.localIP());

  // pruefen, ob OTA moeglich ist
  if (!Arduino_Portenta_OTA::isOtaCapable()) {
    otaMeldung = "OTA nicht moeglich: Bootloader zu alt";
  } else {
    Arduino_Portenta_OTA_QSPI ota(QSPI_FLASH_FATFS_MBR, 2);
    int e = (int)ota.begin();
    otaMeldung = (e == 0) ? "" : "OTA-Speicher fehlt (Fehler " + String(e) + ")";
  }
  if (otaMeldung.length()) Serial.println(otaMeldung); else Serial.println("OTA bereit");
  Serial.print("Firmware "); Serial.print(FW_VERSION); Serial.print(" vom "); Serial.println(FW_DATUM);
}

void webStatusSeriell() {
  Serial.print("WLAN="); Serial.print(wlanOk ? "ok " : "aus ");
  if (wlanOk) { Serial.print("http://"); Serial.print(WiFi.localIP()); Serial.print(" "); }
  Serial.print("Firmware="); Serial.print(FW_VERSION);
  Serial.print(" GitHub="); Serial.print(neuesteVersion.length() ? neuesteVersion : "-");
  Serial.print(autoUpdate() ? " (Auto-Update an)" : " (Auto-Update aus)");
  if (otaMeldung.length()) { Serial.print(" OTA: "); Serial.print(otaMeldung); }
  Serial.println();
}

// ---------- HTTP ----------
String statusJson() {
  String s = "{\"aufzug\":";
  s += aufzugAn ? "true" : "false";
  s += ",\"leds\":";   s += ledsAn ? "true" : "false";
  s += ",\"stopp\":";  s += stoppOffen ? "true" : "false";
  s += ",\"weiche\":"; s += weiche.stellung1 ? 1 : 2;
  s += ",\"hebel\":";  s += hebel.stellung1 ? 1 : 2;
  s += ",\"version\":\""; s += FW_VERSION;
  s += "\",\"ip\":\"";    s += WiFi.localIP().toString();
  s += "\",\"datum\":\"";  s += FW_DATUM;
  s += "\",\"uptime\":";   s += String(millis() / 1000);
  s += ",\"rssi\":";        s += String(WiFi.RSSI());
  s += ",\"github\":\""; s += neuesteVersion;
  s += "\",\"auto\":";     s += autoUpdate() ? "true" : "false";
  s += ",\"ota\":\"";     s += otaMeldung;
  s += "\"}";
  return s;
}

void antwort(WiFiClient& c, const char* typ, const String& body, int code = 200) {
  c.print("HTTP/1.1 "); c.print(code); c.println(code == 200 ? " OK" : " Fehler");
  c.print("Content-Type: "); c.println(typ);
  c.println("Cache-Control: no-store");
  c.print("Content-Length: "); c.println(body.length());
  c.println("Connection: close");
  c.println();
  c.print(body);
}

// %XX in URL-Parametern dekodieren
String urlDecode(const String& in) {
  String out;
  for (unsigned i = 0; i < in.length(); i++) {
    char ch = in[i];
    if (ch == '+') out += ' ';
    else if (ch == '%' && i + 2 < in.length()) {
      out += (char)strtol(in.substring(i + 1, i + 3).c_str(), nullptr, 16);
      i += 2;
    } else out += ch;
  }
  return out;
}

String param(const String& pfad, const char* name) {
  String key = String(name) + "=";
  int q = pfad.indexOf('?');
  if (q < 0) return "";
  int p = pfad.indexOf(key, q);
  if (p < 0) return "";
  p += key.length();
  int e = pfad.indexOf('&', p);
  return urlDecode(pfad.substring(p, e < 0 ? pfad.length() : e));
}

// eine Zeile lesen (mit Zeitlimit, damit haengende Clients nichts blockieren)
String zeile(WiFiClient& c, unsigned long timeout) {
  String z;
  unsigned long t0 = millis();
  while (c.connected() && millis() - t0 < timeout) {
    if (c.available()) {
      char ch = c.read();
      if (ch == '\n') return z;
      if (ch != '\r' && z.length() < 300) z += ch;
    }
  }
  return z;
}

void webLoop() {
  if (!wlanOk) return;

  if (otaUrl.length()) { String u = otaUrl; otaUrl = ""; otaStart(u); }
  githubPruefen();

  WiFiClient c = server.accept();
  if (!c) return;

  String req = zeile(c, 500);                 // z. B. "GET /cmd?c=a HTTP/1.1"
  while (c.connected() && zeile(c, 200).length() > 0) {}   // Header ueberspringen

  int a = req.indexOf(' '), b = req.indexOf(' ', a + 1);
  String pfad = (a > 0 && b > a) ? req.substring(a + 1, b) : "/";

  if (pfad == "/" || pfad.startsWith("/?")) {
    String html = HTML_SEITE;
    String vorschlag = "http://" + c.remoteIP().toString() + ":" + OTA_PORT + "/Parcours1.ota";
    html.replace("%OTA_URL%", vorschlag);
    antwort(c, "text/html; charset=utf-8", html);
  } else if (pfad.startsWith("/status")) {
    antwort(c, "application/json", statusJson());
  } else if (pfad.startsWith("/cmd")) {
    String cm = param(pfad, "c");
    if (cm.length() == 1 && strchr("awhslx", cm[0])) befehl(cm[0]);
    antwort(c, "application/json", statusJson());
  } else if (pfad.startsWith("/github")) {
    githubUpdateAnfordern();
    antwort(c, "text/plain; charset=utf-8", "Update von GitHub gestartet");
  } else if (pfad.startsWith("/update")) {
    String u = param(pfad, "url");
    if (u.startsWith("http://") || u.startsWith("https://")) {
      otaUrl = u;
      antwort(c, "text/plain; charset=utf-8", "Update gestartet: " + u);
    } else {
      antwort(c, "text/plain; charset=utf-8", "url fehlt oder ungueltig", 400);
    }
  } else {
    antwort(c, "text/plain", "nicht gefunden", 404);
  }
  delay(2);
  c.stop();
}

// ---------- OTA ----------
void otaStart(const String& url) {
  allesStopp();
  Serial.print("OTA-Update von "); Serial.println(url);
  otaMeldung = "Update laeuft ...";

  if (!Arduino_Portenta_OTA::isOtaCapable()) { otaMeldung = "OTA nicht moeglich: Bootloader zu alt"; Serial.println(otaMeldung); return; }

  Arduino_Portenta_OTA_QSPI ota(QSPI_FLASH_FATFS_MBR, 2);
  Arduino_Portenta_OTA::Error e = ota.begin();
  if (e != Arduino_Portenta_OTA::Error::None) {
    otaMeldung = "OTA-Speicher fehlt (Fehler " + String((int)e) + ")"; Serial.println(otaMeldung); return;
  }

  unsigned long t0 = millis();
  int n;
  if (url.startsWith("https://")) {
    // selbst laden (eigenes Zertifikat), Datei liegt dann dort, wo die Bibliothek sie erwartet
    FILE* f = fopen("/fs/UPDATE.BIN.LZSS", "wb");
    if (!f) { otaMeldung = "OTA-Datei kann nicht angelegt werden"; Serial.println(otaMeldung); return; }
    uint32_t bytes = 0;
    int code = httpsAbruf(url, [f, &bytes](const char* b, uint32_t len) { fwrite(b, 1, len, f); bytes += len; });
    fclose(f);
    if (code != 200) {
      remove("/fs/UPDATE.BIN.LZSS");
      otaMeldung = "Download fehlgeschlagen (HTTP/Code " + String(code) + ")"; Serial.println(otaMeldung); return;
    }
    Serial.print(bytes); Serial.println(" Bytes geladen, pruefe und entpacke ...");
    n = ota.decompress();     // prueft Laenge, CRC und Board-Kennung
  } else {
    n = ota.downloadAndDecompress(url.c_str(), false);
  }
  if (n <= 0) {
    otaMeldung = "Update-Datei ungueltig (Fehler " + String(n) + ")"; Serial.println(otaMeldung); return;
  }
  Serial.print(n); Serial.print(" Bytes in "); Serial.print((millis() - t0) / 1000.0); Serial.println(" s");

  e = ota.update();
  if (e != Arduino_Portenta_OTA::Error::None) {
    otaMeldung = "Update fehlgeschlagen (Fehler " + String((int)e) + ")"; Serial.println(otaMeldung); return;
  }
  Serial.println("Neustart - Bootloader spielt neue Firmware ein");
  delay(500);
  ota.reset();
}

// ---------- Auto-Update von GitHub ----------
// GitHub Actions baut bei jedem Push die Firmware und legt sie mit version.txt
// (Commit-Hash) auf GitHub Pages. Der Giga vergleicht regelmaessig und spielt
// eine neue Version selbst ein. Lokal per USB gebaute Firmware ("lokal")
// aktualisiert sich nicht automatisch, damit lokale Tests nicht ueberschrieben werden.

bool autoUpdate() { return strcmp(FW_VERSION, "lokal") != 0; }

int letzterDownloadCode = 0;

String githubVersion() {
  String text;
  int code = httpsAbruf(String(OTA_BASIS) + "/version.txt", [&text](const char* b, uint32_t len) {
    for (uint32_t i = 0; i < len && text.length() < 64; i++) text += b[i];
  });
  letzterDownloadCode = code;
  text.trim();
  if (code != 200 || text.length() < 7 || text.length() > 40) return "";
  for (unsigned i = 0; i < text.length(); i++) if (!isHexadecimalDigit(text[i])) return "";
  return text;
}

// Update von GitHub beim naechsten loop ausfuehren (Knopf, serieller Befehl 'u')
void githubUpdateAnfordern() { otaUrl = String(OTA_BASIS) + "/Parcours1.ota"; }

void githubPruefen() {
  // nicht pruefen, waehrend eine Weiche faehrt (die Abfrage blockiert ~1-2 s)
  if (weiche.aktiv || hebel.aktiv) return;
  if (!erstePruefung && millis() - letztePruefung < UPDATE_PRUEF_MS) return;
  if (erstePruefung && millis() < 5000) return;   // nach dem Start kurz warten
  erstePruefung = false;
  letztePruefung = millis();

  String v = githubVersion();
  if (v.length() == 0) {
    Serial.print("GitHub: Version nicht abrufbar (Code "); Serial.print(letzterDownloadCode); Serial.println(")");
    return;
  }
  if (v != neuesteVersion) { Serial.print("GitHub: neueste Version "); Serial.println(v); }
  neuesteVersion = v;
  if (autoUpdate() && v != FW_VERSION && v != fehlVersion) {
    Serial.println("GitHub: neue Firmware -> Update");
    otaStart(String(OTA_BASIS) + "/Parcours1.ota");
    fehlVersion = v;   // nur erreicht, wenn das Update fehlschlug (sonst Neustart)
    Serial.println("GitHub: Auto-Update fehlgeschlagen, erst wieder bei neuer Version oder per Knopf");
  }
}

// Diagnose ('g' im seriellen Monitor): Namensaufloesung und HTTPS-Abruf testen
void githubDiagnose() {
  IPAddress ip;
  String host = String(OTA_BASIS);
  host.replace("https://", ""); host = host.substring(0, host.indexOf('/'));
  int r = WiFi.hostByName(host.c_str(), ip);
  Serial.print("DNS "); Serial.print(host); Serial.print(" -> "); Serial.print(r == 1 ? "ok " : "Fehler "); Serial.println(ip);
  String v = githubVersion();
  Serial.print("version.txt: HTTP/Code "); Serial.print(letzterDownloadCode); Serial.print(", Inhalt '"); Serial.print(v); Serial.println("'");
}
