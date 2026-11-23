// ---------- MQTT: Verbindung zum M5Stack-Bedienpanel ----------
// Das M5Stack ist der MQTT-Broker (PicoMQTT, Port 1883) und meldet sich per UDP-Broadcast
// ("P1BROKER" an Port 47900). Der Giga verbindet sich als Client - ganz ohne Internet.
// (Dateiname beginnt mit "Web", damit Arduino sie nach Web.ino einfuegt.)
//
// Die ganze Netzwerkarbeit fuer MQTT laeuft in einem eigenen Thread: Stockt das WLAN
// (Paketverlust im Hotspot -> TCP wartet auf Wiederholung), laufen Anlage, Sensor und
// Webseite trotzdem weiter. Der Thread schickt immer nur den neuesten Status.
// Eingehende Befehle reicht er an die Hauptschleife weiter (dort werden sie ausgefuehrt).
//
// Themen:
//   p1/status   Giga -> M5   Zustand als JSON (wie /status), 4x pro Sekunde
//   p1/einst    Giga -> M5   alle Einstellungen als JSON (wie /einst), nach jeder Aenderung
//   p1/cmd      M5 -> Giga   ein Befehlsbuchstabe (a w h s l x v j)
//   p1/set      M5 -> Giga   "schluessel=wert", z. B. "weiche_ms=800"
//   p1/lampe    M5 -> Giga   Testfarbe der Statuslampe (a g o r b w 0)
//   p1/get      M5 -> Giga   Einstellungen neu senden
//   p1/hb       M5 -> Giga   Lebenszeichen alle 400 ms (Inhalt: WLAN-Staerke des M5).
//                            Bleibt es aus, geht die Anlage in den sicheren Zustand.
#include <ArduinoMqttClient.h>
#include <mbed.h>
#include <vector>

WiFiClient mqttNetz;
MqttClient mqtt(mqttNetz);
WiFiUDP suchUdp;
const int SUCH_PORT = 47900;
const unsigned long M5_TIMEOUT_MS = 5000;   // so lange ohne Lebenszeichen -> sicherer Zustand

rtos::Thread mqttThread(osPriorityBelowNormal, 8192, nullptr, "mqtt");
rtos::Mutex mqttMtx;
String mqttStatusRaus, mqttEinstRaus;       // von der Hauptschleife zum Senden abgelegt
bool statusRaus = false, einstRaus = false;
struct MqttEin { String thema, text; };
std::vector<MqttEin> mqttEin;               // vom Thread empfangen, von der Hauptschleife ausgefuehrt
volatile unsigned long m5Hb = 0;            // Zeit des letzten Lebenszeichens
volatile bool mqttVerbunden = false, brokerBekannt = false;
bool m5Aktiv = false;                       // M5 hat sich gemeldet -> Waechter scharf

String m5Status() {
  return String("\"") + (m5Aktiv ? "ok" : m5Verloren ? "verloren" : mqttVerbunden ? "warte" : brokerBekannt ? "getrennt" : "aus") + "\"";
}

// ----- laeuft im MQTT-Thread -----
void mqttNachricht(int groesse) {
  String thema = mqtt.messageTopic();
  String text;
  while (mqtt.available()) text += (char)mqtt.read();
  if (thema == "p1/hb") { m5Hb = millis(); m5Rssi = text.toInt(); return; }
  mqttMtx.lock(); if (mqttEin.size() < 20) mqttEin.push_back({thema, text}); mqttMtx.unlock();
}

void mqttSenden(const char* thema, const String& text) {
  mqtt.beginMessage(thema, (unsigned long)text.length());
  mqtt.print(text);
  mqtt.endMessage();
}

void mqttThreadLauf() {
  IPAddress brokerIp;
  bool udpAn = suchUdp.begin(SUCH_PORT);
  unsigned long brokerGesehen = 0, versuch = 0, pause = 0;
  mqtt.setId("giga-parcours1");
  mqtt.setKeepAliveInterval(60000);
  mqtt.setConnectionTimeout(3000);
  mqtt.setTxPayloadSize(1100);
  mqtt.onMessage(mqttNachricht);
  for (;;) {
    bool verb = mqtt.connected();
    // Die UDP-Suche belegt einen der wenigen Sockets des Giga: nur offen, solange keine Verbindung besteht
    if (verb && udpAn) { suchUdp.stop(); udpAn = false; }
    if (!verb && !udpAn) udpAn = suchUdp.begin(SUCH_PORT);
    if (udpAn && suchUdp.parsePacket() > 0) {
      char b[32]; int l = suchUdp.read(b, sizeof(b) - 1); b[max(0, l)] = 0;
      if (strncmp(b, "P1BROKER", 8) == 0) {
        IPAddress ip = suchUdp.remoteIP();
        if (!brokerBekannt || ip != brokerIp) { brokerIp = ip; pause = 0; Serial.print("M5Stack-Broker gefunden: "); Serial.println(ip); }
        brokerBekannt = true; brokerGesehen = millis();
      }
    }
    if (!verb) {
      mqttVerbunden = false;
      // nur verbinden, wenn das M5 sich gerade meldet
      if (brokerBekannt && millis() - brokerGesehen < 2500 && millis() - versuch >= pause) {
        versuch = millis();
        unsigned long tc = millis();
        bool ok = mqtt.connect(brokerIp, 1883);
        dConnA = max(dConnA, millis() - tc); verbindungen++;
        if (ok) {
          mqtt.subscribe("p1/cmd"); mqtt.subscribe("p1/set"); mqtt.subscribe("p1/lampe");
          mqtt.subscribe("p1/get"); mqtt.subscribe("p1/hb");
          pause = 2000; einstSenden = true;
          Serial.println("MQTT verbunden");
        } else pause = min(20000UL, pause * 2 + 2000);
      }
      rtos::ThisThread::sleep_for(std::chrono::milliseconds(20));
      continue;
    }
    mqttVerbunden = true;
    unsigned long tp = millis();
    mqtt.poll();
    dPollA = max(dPollA, millis() - tp);
    // neuesten Status / Einstellungen abholen und senden
    String st, ei; bool s1 = false, s2 = false;
    mqttMtx.lock();
    if (statusRaus) { st = mqttStatusRaus; statusRaus = false; s1 = true; }
    if (einstRaus) { ei = mqttEinstRaus; einstRaus = false; s2 = true; }
    mqttMtx.unlock();
    unsigned long ts = millis();
    if (s2) mqttSenden("p1/einst", ei);
    if (s1) mqttSenden("p1/status", st);
    dSendA = max(dSendA, millis() - ts);
    rtos::ThisThread::sleep_for(std::chrono::milliseconds(5));
  }
}

// ----- laeuft in der Hauptschleife -----
void mqttSetup() {
  if (!wlanOk) return;
  mqttThread.start(mqttThreadLauf);
}

// Waechter: das M5 war verbunden und meldet sich nicht mehr -> alles stoppen
void m5Waechter() {
  unsigned long hb = m5Hb;
  if (!m5Aktiv) {
    if (hb && millis() - hb < 1000) { m5Aktiv = true; einstSenden = true; Serial.println("M5Stack verbunden"); }
    return;
  }
  if (millis() - hb < M5_TIMEOUT_MS) return;
  m5Aktiv = false;
  if (M5_WAECHTER) {
    allesStopp();
    m5Verloren = true;
    Serial.println("STOERUNG: Verbindung zum M5Stack verloren -> sicherer Zustand (alles aus)");
  } else Serial.println("M5Stack getrennt");
}

void mqttLoop() {
  if (!wlanOk) return;
  // War die Hauptschleife selbst blockiert (z. B. GitHub-Abfrage), ist das kein Ausfall des M5
  static unsigned long letzterLauf = 0, statusT = 0;
  if (millis() - letzterLauf > 400 && m5Aktiv) m5Hb = millis();
  letzterLauf = millis();
  if (millis() - dMT > 3000) { dMT = millis(); dPoll = dPollA; dSend = dSendA; dConn = dConnA; dPollA = dSendA = dConnA = 0; }

  // empfangene Befehle ausfuehren
  std::vector<MqttEin> ein;
  mqttMtx.lock(); ein.swap(mqttEin); mqttMtx.unlock();
  for (auto& m : ein) {
    if (m.thema == "p1/cmd" && m.text.length() == 1 && strchr("awhslxvj", m.text[0])) { befehl(m.text[0]); statusT = 0; }
    else if (m.thema == "p1/set") {
      int e = m.text.indexOf('=');
      if (e > 0 && einstSetzen(m.text.substring(0, e), m.text.substring(e + 1).toInt())) { einstAnwenden(); einstSpeichern(); }
      einstSenden = true;
    }
    else if (m.thema == "p1/lampe" && m.text.length() == 1 && strchr("agorbw0", m.text[0])) { lampeModus = m.text[0]; statusT = 0; }
    else if (m.thema == "p1/get") einstSenden = true;
  }
  m5Waechter();

  // Status und Einstellungen fuer den Thread ablegen (aeltere, noch nicht gesendete werden ersetzt)
  if (!mqttVerbunden) return;
  if (einstSenden) { einstSenden = false; String e = einstJson(); mqttMtx.lock(); mqttEinstRaus = e; einstRaus = true; mqttMtx.unlock(); }
  if (millis() - statusT >= 250) {
    statusT = millis();
    String s = statusJson();
    mqttMtx.lock(); mqttStatusRaus = s; statusRaus = true; mqttMtx.unlock();
  }
}
