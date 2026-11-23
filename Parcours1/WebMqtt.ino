// ---------- MQTT: Verbindung zum M5Stack-Bedienpanel ----------
// Das M5Stack ist der MQTT-Broker (PicoMQTT, Port 1883) und meldet sich per UDP-Broadcast
// ("P1BROKER" an Port 47900). Der Giga verbindet sich als Client - ganz ohne Internet.
// (Dateiname beginnt mit "Web", damit Arduino sie nach Web.ino einfuegt.)
//
// Themen:
//   p1/status   Giga -> M5   Zustand als JSON (wie /status), 4x pro Sekunde
//   p1/einst    Giga -> M5   alle Einstellungen als JSON (wie /einst), nach jeder Aenderung
//   p1/cmd      M5 -> Giga   ein Befehlsbuchstabe (a w h s l x v j)
//   p1/set      M5 -> Giga   "schluessel=wert", z. B. "weiche_ms=800"
//   p1/lampe    M5 -> Giga   Testfarbe der Statuslampe (a g o r b w 0)
//   p1/get      M5 -> Giga   Einstellungen neu senden
//   p1/hb       M5 -> Giga   Lebenszeichen alle 500 ms. Bleibt es aus, geht die Anlage in den sicheren Zustand.
#include <ArduinoMqttClient.h>

WiFiClient mqttNetz;
MqttClient mqtt(mqttNetz);
WiFiUDP suchUdp;
const int SUCH_PORT = 47900;
const unsigned long M5_TIMEOUT_MS = 2500;
IPAddress brokerIp;
bool brokerBekannt = false;
unsigned long brokerGesehen = 0, mqttVersuch = 0, mqttPause = 0, statusT = 0, m5Hb = 0;
bool m5Aktiv = false;           // M5 hat sich gemeldet -> Waechter scharf

String m5Status() {
  return String("\"") + (m5Aktiv ? "ok" : m5Verloren ? "verloren" : mqtt.connected() ? "warte" : brokerBekannt ? "getrennt" : "aus") + "\"";
}

void mqttSenden(const char* thema, const String& text) {
  if (!mqtt.connected()) return;
  mqtt.beginMessage(thema, (unsigned long)text.length());
  mqtt.print(text);
  mqtt.endMessage();
}

void mqttNachricht(int groesse) {
  String thema = mqtt.messageTopic();
  String text;
  while (mqtt.available()) text += (char)mqtt.read();
  if (thema == "p1/hb") {
    m5Hb = millis();
    if (!m5Aktiv) { m5Aktiv = true; Serial.println("M5Stack verbunden"); einstSenden = true; }
    return;
  }
  if (thema == "p1/cmd" && text.length() == 1 && strchr("awhslxvj", text[0])) { befehl(text[0]); statusT = 0; }
  else if (thema == "p1/set") {
    int e = text.indexOf('=');
    if (e > 0 && einstSetzen(text.substring(0, e), text.substring(e + 1).toInt())) { einstAnwenden(); einstSpeichern(); }
    einstSenden = true;
  }
  else if (thema == "p1/lampe" && text.length() == 1 && strchr("agorbw0", text[0])) { lampeModus = text[0]; statusT = 0; }
  else if (thema == "p1/get") einstSenden = true;
}

void mqttSetup() {
  if (!wlanOk) return;
  suchUdp.begin(SUCH_PORT);
  mqtt.setId("giga-parcours1");
  mqtt.setKeepAliveInterval(60000);   // eigene Pausen (z. B. GitHub-Abfrage) duerfen die Verbindung nicht kappen
  mqtt.setConnectionTimeout(1500);
  mqtt.setTxPayloadSize(1100);
  mqtt.onMessage(mqttNachricht);
}

// Waechter: das M5 war verbunden und meldet sich nicht mehr -> alles stoppen
void m5Waechter() {
  if (!m5Aktiv || millis() - m5Hb < M5_TIMEOUT_MS) return;
  m5Aktiv = false;
  if (M5_WAECHTER) {
    allesStopp();
    m5Verloren = true;
    Serial.println("STOERUNG: Verbindung zum M5Stack verloren -> sicherer Zustand (alles aus)");
  } else Serial.println("M5Stack getrennt");
}

void mqttLoop() {
  if (!wlanOk) return;
  // War der Giga selbst blockiert (HTTPS-Abfrage, Update), ist das kein Ausfall des M5:
  // Lebenszeichen-Uhr dann neu starten, damit der Waechter nicht faelschlich ausloest.
  static unsigned long letzterLauf = 0;
  if (millis() - letzterLauf > 400) m5Hb = millis();
  letzterLauf = millis();
  // Broker suchen bzw. Adresse aktuell halten
  int n = suchUdp.parsePacket();
  if (n > 0) {
    char b[32]; int l = suchUdp.read(b, sizeof(b) - 1); b[max(0, l)] = 0;
    if (strncmp(b, "P1BROKER", 8) == 0) {
      IPAddress ip = suchUdp.remoteIP();
      if (!brokerBekannt || ip != brokerIp) { brokerIp = ip; mqttPause = 0; Serial.print("M5Stack-Broker gefunden: "); Serial.println(ip); }
      brokerBekannt = true; brokerGesehen = millis();
    }
  }
  if (mqtt.connected()) mqtt.poll();   // vor dem Waechter: ein wartendes Lebenszeichen zaehlt noch
  m5Waechter();
  if (!brokerBekannt) return;

  if (!mqtt.connected()) {
    // Nur verbinden, wenn das M5 sich gerade per UDP meldet: ein Verbindungsversuch zu einem
    // ausgeschalteten M5 blockiert den Giga sonst viele Sekunden (Webseite, Sensor, Servo).
    if (millis() - brokerGesehen > 2500) return;
    if (millis() - mqttVersuch < mqttPause) return;
    mqttVersuch = millis();
    if (mqtt.connect(brokerIp, 1883)) {
      mqtt.subscribe("p1/cmd"); mqtt.subscribe("p1/set"); mqtt.subscribe("p1/lampe");
      mqtt.subscribe("p1/get"); mqtt.subscribe("p1/hb");
      mqttPause = 2000; einstSenden = true;
      Serial.println("MQTT verbunden");
    } else {
      mqttPause = min(20000UL, mqttPause * 2 + 2000);   // nicht dauernd blockieren, wenn das M5 aus ist
      if (millis() - brokerGesehen > 10000) brokerBekannt = false;
    }
    return;
  }
  if (einstSenden) { einstSenden = false; mqttSenden("p1/einst", einstJson()); }
  if (millis() - statusT >= 250) { statusT = millis(); mqttSenden("p1/status", statusJson()); }
}
