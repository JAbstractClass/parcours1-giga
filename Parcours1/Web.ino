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
#include <Arduino_Portenta_OTA.h>
#include "arduino_secrets.h"

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

static const char HTML_SEITE[] = R"HTML(<!doctype html>
<html lang="de"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Parcours 1</title>
<style>
:root{--bg:#f4f5f7;--card:#fff;--text:#1d2330;--muted:#667085;--on:#1f9d55;--off:#c0c6d0;--accent:#2563eb;--warn:#b42318}
@media (prefers-color-scheme:dark){:root{--bg:#12151b;--card:#1c212b;--text:#e8ebf0;--muted:#98a2b3;--off:#3a4250}}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--text);font:16px/1.4 system-ui,sans-serif}
main{max-width:560px;margin:0 auto;padding:16px}
h1{font-size:22px;margin:4px 0 2px}.sub{color:var(--muted);font-size:13px;margin-bottom:14px}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:10px}
button{font:inherit;border:0;border-radius:12px;padding:16px 12px;background:var(--card);color:var(--text);
box-shadow:0 1px 3px #0002;cursor:pointer;text-align:left;min-height:72px}
button:active{transform:scale(.98)}
button b{display:block;font-size:16px}button span{font-size:13px;color:var(--muted)}
.dot{display:inline-block;width:10px;height:10px;border-radius:50%;background:var(--off);margin-right:6px;vertical-align:middle}
.on .dot{background:var(--on)}
.stop{grid-column:1/-1;background:var(--warn);color:#fff;text-align:center;min-height:56px}
.stop span{color:#fff}
.card{background:var(--card);border-radius:12px;padding:14px;margin-top:14px;box-shadow:0 1px 3px #0002}
.card h2{font-size:15px;margin:0 0 8px}
input{width:100%;font:inherit;padding:10px;border-radius:8px;border:1px solid var(--off);background:var(--bg);color:var(--text)}
.row{display:flex;gap:8px;margin-top:8px}.row button{min-height:0;padding:10px 14px;text-align:center;background:var(--accent);color:#fff}
#msg{font-size:13px;color:var(--muted);margin-top:8px;min-height:1em}
.offline{color:var(--warn)}
</style></head><body><main>
<h1>Parcours 1</h1>
<div class="sub" id="info">verbinde …</div>
<div class="grid">
 <button id="b_a" onclick="cmd('a')"><b><i class="dot"></i>Aufzug</b><span id="s_a">–</span></button>
 <button id="b_l" onclick="cmd('l')"><b><i class="dot"></i>LEDs</b><span id="s_l">–</span></button>
 <button onclick="cmd('w')"><b>Stellweiche</b><span id="s_w">–</span></button>
 <button onclick="cmd('h')"><b>Weichenhebel</b><span id="s_h">–</span></button>
 <button id="b_s" onclick="cmd('s')"><b><i class="dot"></i>Stoppstelle</b><span id="s_s">–</span></button>
 <button class="stop" onclick="cmd('x')"><b>Alles stoppen</b></button>
</div>
<div class="card">
 <h2>Firmware-Update</h2>
 <div class="sub" id="gh">GitHub: –</div>
 <div class="row"><button onclick="gh()">Jetzt von GitHub laden</button></div>
 <h2 style="margin-top:14px">Update von eigener Adresse</h2>
 <input id="url" spellcheck="false">
 <div class="row"><button onclick="upd()">Update starten</button></div>
 <div id="msg"></div>
</div>
</main><script>
let ver=null;
const $=id=>document.getElementById(id);
function show(s){
 $('info').className='sub';
 $('info').textContent='Firmware '+s.version+' · '+s.ip+(s.ota?' · '+s.ota:'');
 $('b_a').className=s.aufzug?'on':'';$('s_a').textContent=s.aufzug?'läuft':'aus';
 $('b_l').className=s.leds?'on':'';$('s_l').textContent=s.leds?'an':'aus';
 $('b_s').className=s.stopp?'on':'';$('s_s').textContent=s.stopp?'offen':'zu';
 $('s_w').textContent='Stellung '+s.weiche;$('s_h').textContent='Stellung '+s.hebel;
 if(ver&&ver!==s.version)$('msg').textContent='Neue Firmware läuft: '+s.version;
 ver=s.version;
 $('gh').textContent='GitHub: '+(s.github||'noch nicht geprüft')+(s.auto?' · automatisch an':' · automatisch aus (lokale Firmware)');
}
async function poll(){
 try{const r=await fetch('/status',{cache:'no-store'});show(await r.json());}
 catch(e){$('info').className='sub offline';$('info').textContent='keine Verbindung zum Giga …';}
}
async function cmd(c){try{const r=await fetch('/cmd?c='+c);show(await r.json());}catch(e){}}
async function upd(){
 const u=$('url').value.trim();if(!u)return;
 $('msg').textContent='Update läuft – Giga lädt Firmware und startet neu (ca. 30 s) …';
 try{await fetch('/update?url='+encodeURIComponent(u));}catch(e){}
}
async function gh(){
 $('msg').textContent='Update von GitHub läuft – Giga lädt Firmware und startet neu (ca. 30 s) …';
 try{await fetch('/github');}catch(e){}
}
$('url').value='%OTA_URL%';
poll();setInterval(poll,1000);
</script></body></html>
)HTML";

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
  s += "\",\"github\":\""; s += neuesteVersion;
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
    otaUrl = String(OTA_BASIS) + "/Parcours1.ota";
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
  int n = ota.downloadAndDecompress(url.c_str(), url.startsWith("https://"));
  if (n <= 0) {
    otaMeldung = "Download fehlgeschlagen (Fehler " + String(n) + ")"; Serial.println(otaMeldung); return;
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

String githubVersion() {
  String text;
  String url = String(OTA_BASIS) + "/version.txt";
  int n = WiFi.download((char*)url.c_str(), true, [&text](const char* b, uint32_t len) {
    for (uint32_t i = 0; i < len && text.length() < 64; i++) text += b[i];
  });
  text.trim();
  if (n <= 0 || text.length() < 7 || text.length() > 40) return "";
  for (unsigned i = 0; i < text.length(); i++) if (!isHexadecimalDigit(text[i])) return "";
  return text;
}

void githubPruefen() {
  // nicht pruefen, waehrend eine Weiche faehrt (die Abfrage blockiert ~1-2 s)
  if (weiche.aktiv || hebel.aktiv) return;
  if (!erstePruefung && millis() - letztePruefung < UPDATE_PRUEF_MS) return;
  if (erstePruefung && millis() < 5000) return;   // nach dem Start kurz warten
  erstePruefung = false;
  letztePruefung = millis();

  String v = githubVersion();
  if (v.length() == 0) { Serial.println("GitHub: Version nicht abrufbar"); return; }
  if (v != neuesteVersion) { Serial.print("GitHub: neueste Version "); Serial.println(v); }
  neuesteVersion = v;
  if (autoUpdate() && v != FW_VERSION && v != fehlVersion) {
    Serial.println("GitHub: neue Firmware -> Update");
    otaStart(String(OTA_BASIS) + "/Parcours1.ota");
    fehlVersion = v;   // nur erreicht, wenn das Update fehlschlug (sonst Neustart)
    Serial.println("GitHub: Auto-Update fehlgeschlagen, erst wieder bei neuer Version oder per Knopf");
  }
}
