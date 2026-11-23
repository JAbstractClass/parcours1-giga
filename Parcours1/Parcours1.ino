// Parcours 1 - fischertechnik Dynamic XXL
// Mega-F5 Shield auf Arduino Giga R1 WiFi
// Belegung laut "Verkabelung_Parcours1_Arduino 2.pdf" (inkl. handschriftlicher Taster-Notiz)
//
// Taster: beim Start NICHT druecken - der Ruhezustand wird dann gemessen.
// Bedienung per Taster:
//   T_M (Mitte)  -> Kettenaufzug AN/AUS
//   T_L (links)  -> Sprechtaste: gedrueckt halten und ins Mikrofon (A7) sprechen
//                   (Weiche 1 schaltet man jetzt im Dashboard oder am M5)
//   T_R (rechts) -> Stellweiche mit Weichenhebel umschalten
// Stoppstelle: oeffnet automatisch alle STOPP_INTERVALL_MS kurz, solange der Aufzug laeuft.
//
// Serieller Monitor (115200 Baud):
//   a = Aufzug   w = Stellweiche   h = Weichenhebel   s = Stoppstelle
//   l = LEDs     t = Taster-Rohwerte   ? = Status
//   m = Taster-Rohwerte live an/aus      p = alle Analogpins
//   x = alles stoppen   n = WLANs in Reichweite anzeigen
//   g = GitHub-Verbindung testen   u = Update von GitHub jetzt laden   v = Servo-Test   j = Weihnachtsmodus
//
// WLAN, Weboberflaeche und OTA-Update: siehe Web.ino und arduino_secrets.h

#include <Servo.h>
#include <Arduino_AdvancedAnalog.h>
#include "Typen.h"
#include <mbed.h>
#include <WiFi.h>   // hier, damit die Arduino-Prototypen fuer Web.ino WiFiClient kennen

// ---------- Pinbelegung ----------
const int AUFZUG_A    = 4;   // M2 (PWM)
const int AUFZUG_B    = 5;   // M2 (PWM)
const int WEICHE_A    = 6;   // M3 Stellweiche Flexschiene
const int WEICHE_B    = 7;   // M3
const int HEBEL_A     = 8;   // M4 Stellweiche mit Weichenhebel
const int HEBEL_B     = 9;   // M4
const int LED_1       = 16;  // M1 Klemme 16 -> LED -> GND
const int LED_2       = 17;  // M1 Klemme 17 -> LED -> GND
const int SERVO_STOPP = 2;   // SERVO 1 (3-polig: Signal an ~2, VCC, GND)
const int SERVO_STOPP2 = 3;  // SERVO 2: bekommt dasselbe Signal, falls der Servo dort steckt
// Optional: RGB-LED-Modul (Sensorkit B09) als Statuslampe oben am Turm, "-" an GND.
// Beide Anschlussmoeglichkeiten werden gleichzeitig angesteuert:
//   R -> IN A0 oder D18,  G -> IN A1 oder D19,  B -> IN A2
const int RGB_R[] = {A0, 18};
const int RGB_G[] = {A1, 19};
const int RGB_B[] = {A2};

// Kugelsensor an der Stoppstelle, OUT an IN A6, Versorgung 3,3 V (nie 5 V - A6 vertraegt hoechstens 3,3 V!).
// Zwei Arten (Einstellung sensor_typ):
//   0 = Reflex-/Lichtsensor (B04): meldet LOW, solange eine Kugel davor liegt
//   1 = Erschuetterungssensor (Shock, B23): kurzer Impuls, wenn eine Kugel aufschlaegt -> per Interrupt gezaehlt
const int SENSOR_PINS[] = {A6};
const char* SENSOR_NAMEN[] = {"A6"};
const int N_SENSOR = 1;
// Lautsprecher (2 Pins): ein Pin an P15 der 8er-Stiftleiste unten (= D15), der andere an GND.
// Unbedingt mindestens 470 Ohm in Reihe: der Giga-Pin liefert nur wenige mA (16-Ohm-Lautsprecher direkt = Pin kaputt).
// D15 liefert das Audiosignal als schnelle PWM. Teilt sich der Timer mit einem Motor-Pin, laeuft dessen PWM
// waehrend der Musik ebenfalls schnell; nach der Musik wird der Takt wieder auf 500 Hz gesetzt.
const int LAUTSPR_A = 15;      // PWM
// Fuer deutlich mehr Lautstaerke: Lautsprecher stattdessen an M6 (Buchsen D12/D13) mit ca. 100 Ohm in Reihe.
// Dort treibt der Motortreiber ihn mit 9 V. Beide Ausgaenge spielen gleichzeitig.
const int LAUTSPR_M6 = 13;     // PWM (Timer TIM12)
const int LAUTSPR_M6_GND = 12; // bleibt LOW

const int TASTER_L = A5;     // T_L = Sprechtaste
const int MIKROFON = A7;     // Mikrofon-Modul, analoger Ausgang
const int TASTER_M = A4;     // T_M
const int TASTER_R = A3;     // T_R

// ---------- Kalibrierung ----------
// Startwerte. Im Dashboard unter "Einstellen" live aenderbar; geaenderte Werte
// speichert der Giga im Flash (/fs/einst.txt), sie gelten auch nach Neustart und Update.
int AUFZUG_PWM         = 200;   // 0..255, Geschwindigkeit Kettenaufzug
int AUFZUG_UMKEHREN    = 0;     // 1, falls Aufzug falsch herum laeuft
// Weichenmotoren: Kraft (0..255) und Fahrzeit bis Anschlag, je Weiche einzeln.
// Kommt eine Weiche nicht bis zum Anschlag: Fahrzeit erhoehen. Brummt sie am Anschlag: verringern.
int WEICHE_PWM         = 255;   // Weiche 1 (Flexschiene, M3)
int WEICHE_PULS_MS     = 750;
int WEICHE_UMKEHREN    = 0;
int HEBEL_PWM          = 180;   // Weiche 2 (Weichenhebel, M4)
int HEBEL_PULS_MS      = 350;
int HEBEL_UMKEHREN     = 0;
// Rainbow-LEDs koennen ihre Farbe nicht waehlen -> Zustand als Blinkcode:
// Dauerlicht = alles ok, langsam blinken = kein Internet, schnell blinken = Fehler / Not-Aus
int LED_BLINKCODE      = 1;
int STOPP_ZU           = 60;    // Servo-Winkel: Kugel gestoppt
int STOPP_AUF          = 120;   // Servo-Winkel: Kugel frei
int STOPP_OFFEN_MS     = 180;   // Freigabe: so lange offen, dann sofort wieder zu
int STOPP_TEMPO        = 0;     // 0 = Servo faehrt so schnell er kann, sonst Grad pro 10 ms
int STOPP_AUTOMATIK    = 1;     // Stoppstelle automatisch takten, solange der Aufzug laeuft
int STOPP_INTERVALL_MS = 8000;  // Abstand zwischen automatischen Freigaben
int SENSOR_AN          = 1;     // 1 = Stoppstelle gibt nur frei, wenn der Sensor eine Kugel sieht
int SENSOR_INV         = 0;     // 1, falls der Sensor "Kugel" und "keine Kugel" vertauscht meldet
int STOPP_NACHLAUF_MS  = 120;   // mit Sensor: so lange nach dem Durchrollen noch offen, dann zu
int SENSOR_TYP         = 1;     // 0 = Reflex (Kugel liegt davor), 1 = Shock (Kugel schlaegt auf)
int SENSOR_SPERR_MS    = 150;   // Shock: Mindestabstand zwischen zwei Kugeln (Prellen der Feder ausblenden)
int STAU_S             = 30;    // Kugelstau melden, wenn der Aufzug so lange laeuft, ohne dass eine Kugel ankommt (0 = aus)
int M5_WAECHTER        = 1;     // 1 = Anlage stoppt, wenn das verbundene M5Stack nicht mehr antwortet
int SENSOR_EMPF        = 10;    // Shock: Empfindlichkeit 1..10. 10 = schon ein einziger Kontakt-Zucker zaehlt,
                                // niedriger = es braucht mehr Zucker innerhalb von 50 ms (filtert Motor-Vibration)
const unsigned long ENTPRELL_MS = 40;

// ---------- Zustand ----------
Servo stopp, stopp2;
int servoIst = -1, servoSoll = 60;     // aktueller und Ziel-Winkel
unsigned long servoSchritt = 0;
void servoSchreiben(int w) { servoIst = w; stopp.write(w); stopp2.write(w); }
// Ziel setzen; mit STOPP_TEMPO > 0 faehrt servoUpdate() langsam hin
void servoWinkel(int w) {
  servoSoll = constrain(w, 0, 180);
  if (STOPP_TEMPO <= 0 || servoIst < 0) servoSchreiben(servoSoll);
}
void servoUpdate() {
  if (servoIst == servoSoll || millis() - servoSchritt < 10) return;
  servoSchritt = millis();
  int d = servoSoll - servoIst, s = max(1, STOPP_TEMPO);
  servoSchreiben(servoIst + constrain(d, -s, s));
}
bool aufzugAn = false, ledsAn = true, stoppOffen = false;
bool notAus = false;            // 'x' gedrueckt: bleibt rot, bis der Aufzug wieder gestartet wird
bool m5Verloren = false;        // Verbindung zum M5Stack weg -> sicherer Zustand (WebMqtt.ino)
bool einstSenden = false;
String statusJson();       // Einstellungen per MQTT neu an das M5 schicken
bool stau = false;              // Stoerung: Aufzug laeuft, aber seit STAU_S keine Kugel
unsigned long letzteKugelT = 0;
unsigned long laufMs = 0, laufSeit = 0;   // Betriebszeit des Aufzugs seit Neustart
// Diagnose: laengste Dauer je Teil der Hauptschleife in den letzten 3 s (im Status als "diag")
unsigned long dWeb = 0, dMqtt = 0, dRest = 0, dWebA = 0, dMqttA = 0, dRestA = 0, dT = 0;
unsigned long dPoll = 0, dSend = 0, dConn = 0, dPollA = 0, dSendA = 0, dConnA = 0, dMT = 0; uint32_t verbindungen = 0;
volatile int m5Rssi = 0;       // WLAN-Staerke des M5 (kommt mit dem Lebenszeichen)
bool weicheLinks = true, hebelLinks = true;
unsigned long stoppZeit = 0;
bool live = false;              // 'm': Tasterwerte laufend ausgeben
unsigned long liveZeit = 0;

// speed: -255..255 (Vorzeichen = Richtung, 0 = aus)
void motor(int pinA, int pinB, int speed) {
  analogWrite(pinA, speed > 0 ? speed : 0);
  analogWrite(pinB, speed < 0 ? -speed : 0);
}

// ---------- Weichen: nicht-blockierender Stellpuls ----------
Stellmotor weiche = {WEICHE_A, WEICHE_B, "Weiche", true, 0, false, &WEICHE_PWM, &WEICHE_PULS_MS, &WEICHE_UMKEHREN};
Stellmotor hebel  = {HEBEL_A,  HEBEL_B,  "Hebel",  true, 0, false, &HEBEL_PWM,  &HEBEL_PULS_MS,  &HEBEL_UMKEHREN};

void umschalten(Stellmotor& m) {
  if (m.aktiv) return;                 // laeuft noch
  m.stellung1 = !m.stellung1;
  int p = (m.stellung1 != (*m.umk != 0)) ? *m.pwm : -*m.pwm;
  motor(m.pinA, m.pinB, p);
  m.start = millis();
  m.aktiv = true;
  Serial.print(m.name);
  Serial.println(m.stellung1 ? ": Stellung 1" : ": Stellung 2");
}

void stellUpdate(Stellmotor& m) {
  if (m.aktiv && millis() - m.start >= (unsigned long)*m.pulsMs) {
    motor(m.pinA, m.pinB, 0);
    m.aktiv = false;
  }
}

// ---------- Taster (zwischen A-Pin und GND) ----------
Taster tL = {TASTER_L, false, false, false, 0};
Taster tM = {TASTER_M, false, false, false, 0};
Taster tR = {TASTER_R, false, false, false, 0};

// Taster digital lesen (interner Pull-up; Taster schaltet nach GND -> LOW).
// Analog geht hier nicht zuverlaessig: alle Analogpins zeigen mit dem Shield konstant 1023.
bool pegel(int pin) { return digitalRead(pin) == HIGH; }

void tasterInit(Taster& t) {
  pinMode(t.pin, INPUT_PULLUP);
  delay(5);
  t.ruhe = t.roh = t.zustand = pegel(t.pin);
  t.seit = millis();
}

// liefert true genau beim Druecken (Wechsel weg vom Ruhepegel)
bool gedrueckt(Taster& t) {
  bool r = pegel(t.pin);
  if (r != t.roh) { t.roh = r; t.seit = millis(); }
  if (millis() - t.seit >= ENTPRELL_MS && t.zustand != t.roh) {
    t.zustand = t.roh;
    return t.zustand != t.ruhe;
  }
  return false;
}

// ---------- Funktionen ----------
void setAufzug(bool an) {
  if (an && !aufzugAn) laufSeit = millis();
  if (!an && aufzugAn) laufMs += millis() - laufSeit;
  aufzugAn = an;
  if (an) { notAus = false; m5Verloren = false; stau = false; letzteKugelT = millis(); }   // Starten quittiert Stoerungen
  int v = an ? AUFZUG_PWM : 0;
  motor(AUFZUG_A, AUFZUG_B, AUFZUG_UMKEHREN ? -v : v);
  stoppZeit = millis();
  Serial.println(an ? "Aufzug AN" : "Aufzug AUS");
}

void setLeds(bool an) {
  ledsAn = an;
  ledUpdate();
  Serial.println(an ? "LEDs AN" : "LEDs AUS");
}

void setStopp(bool offen) {
  stoppOffen = offen;
  servoWinkel(offen ? STOPP_AUF : STOPP_ZU);
  stoppZeit = millis();
  Serial.println(offen ? "Stoppstelle offen" : "Stoppstelle zu");
}

// ---------- Kugelsensor ----------
bool kugelDa = false, kugelRoh = false;
unsigned long kugelSeit = 0, kugelWeg = 0;
uint32_t kugelZaehler = 0;      // Kugeln, die durch die Stoppstelle gerollt sind (seit Neustart)

bool sensorRoh() {
  bool r = false;                              // Modul zieht DOUT auf LOW, wenn es etwas sieht
  for (int i = 0; i < N_SENSOR; i++) if (digitalRead(SENSOR_PINS[i]) == LOW) r = true;
  return SENSOR_INV ? !r : r;
}

// Shock: Impulse sind oft kuerzer als ein loop-Durchlauf -> jede Flanke per Interrupt mitzaehlen
volatile uint32_t stossFlanken = 0;
uint32_t stossGesehen = 0;
unsigned long stossZeit = 0;
void stossIsr() { stossFlanken++; }

void sensorUpdate() {
  if (SENSOR_TYP == 1) {
    uint32_t f = stossFlanken;
    static uint32_t burstF = 0; static unsigned long burstT = 0; static bool burst = false;
    if (burst && millis() - burstT > 50) burst = false;               // zu wenig Zucker: war nur Rauschen
    if (f != stossGesehen) {
      if (!burst) { burst = true; burstT = millis(); burstF = stossGesehen; }
      stossGesehen = f;
      uint32_t noetig = 11 - constrain(SENSOR_EMPF, 1, 10);
      if (f - burstF >= noetig && millis() - stossZeit >= (unsigned long)SENSOR_SPERR_MS) {   // neue Kugel, nicht nur Nachschwingen
        burst = false;
        stossZeit = millis();
        kugelDa = true;                               // wartet jetzt an der Stoppstelle, bis freigegeben wird
        if (SENSOR_AN) { kugelZaehler++; letzteKugelT = millis(); stau = false; Serial.print("Kugel angekommen (Stoss), Zaehler "); Serial.println(kugelZaehler); }
      }
    }
    return;
  }
  bool r = sensorRoh();
  if (r != kugelRoh) { kugelRoh = r; kugelSeit = millis(); }
  if (millis() - kugelSeit < 25 || kugelDa == kugelRoh) return;   // entprellen
  kugelDa = kugelRoh;
  if (!kugelDa) {                                 // Kugel hat den Sensor verlassen = durchgerollt
    kugelWeg = millis();
    if (SENSOR_AN) { kugelZaehler++; letzteKugelT = millis(); stau = false; Serial.print("Kugel durch, Zaehler "); Serial.println(kugelZaehler); }
  }
}

// ---------- Sprechtaste und Mikrofon ----------
// Das Mikrofon an A7 wird per DMA mit 8 kHz gelesen (Arduino_AdvancedAnalog), unabhaengig davon,
// wie lange ein loop-Durchlauf dauert (Puffer fuer 2 s). Solange die Sprechtaste gehalten wird,
// sammelt der Giga die Aufnahme in seinem Speicher (max. 30 s) und zeigt dem M5 den Pegel.
// Beim Loslassen schickt er sie selbst per HTTPS an den Server (jl-parcours, Spracherkennung + Claude)
// und gibt dem M5 die Job-Nummer weiter; das M5 zeigt Claudes Schritte und liest die Antwort vor.
// (Das M5 hat zu wenig Speicher, um die Aufnahme selbst verschluesselt hochzuladen.)
AdvancedADC mikro(MIKROFON);
bool mikroAn = false, sprechen = false;
int MIC_GAIN = 6;                                  // Verstaerkung vor der Wandlung auf 8 Bit
int micPP = 0, micMitte = 0;
float micDc = 2048;                                // laufender Mittelwert (Gleichanteil)
int micMin = 4095, micMax = 0; unsigned long micT = 0, micSum = 0, micN = 0;
const uint32_t AUDIO_MAX = 30UL * 8000;            // hoechstens 30 s
uint8_t aufnahme[AUDIO_MAX];
uint32_t audioGesamt = 0; int audioSpitze = 0; unsigned long pegelT = 0;
String p1Token = "";                               // Zugang zum Server, kommt vom M5 per MQTT (steht nicht im Repo)
const char* P1_STIMME_URL = "https://minime.love66.app/p1/stimme";

void mikroSetup() {
  mikroAn = mikro.begin(AN_RESOLUTION_12, 8000, 256, 64);
  Serial.println(mikroAn ? "Mikrofon A7: 8 kHz" : "Mikrofon A7: AdvancedADC startet nicht");
}

void sprechtasteUpdate() {
  gedrueckt(tL);                                   // Entprellung weiterlaufen lassen
  bool g = tL.zustand != tL.ruhe;
  if (g && !sprechen) {
    sprechen = true; audioGesamt = 0;
    Serial.println("Sprechtaste gedrueckt");
    audioSenden("p1/audio/start", (const uint8_t*)"8000", 4);
  }
  if (mikroAn) {
    while (mikro.available()) {
      SampleBuffer buf = mikro.read();
      for (size_t i = 0; i < buf.size(); i++) {
        int v = buf[i];
        micMin = min(micMin, v); micMax = max(micMax, v); micSum += v; micN++;
        micDc += (v - micDc) * 0.002f;
        if (sprechen && audioGesamt < AUDIO_MAX) {
          int s = 128 + (int)((v - micDc) * MIC_GAIN / 16);
          s = constrain(s, 0, 255);
          aufnahme[audioGesamt++] = (uint8_t)s;
          audioSpitze = max(audioSpitze, abs(s - 128));
        }
      }
      buf.release();
    }
  }
  if (sprechen && millis() - pegelT >= 150) {      // Pegel fuers M5 (Claude-Symbol pulsiert)
    pegelT = millis(); String p = String(audioSpitze); audioSpitze = 0;
    audioSenden("p1/audio/pegel", (const uint8_t*)p.c_str(), p.length());
  }
  if (!g && sprechen) {
    sprechen = false;
    Serial.print("Sprechtaste losgelassen, "); Serial.print(audioGesamt / 8000.0, 1); Serial.println(" s Audio");
    stimmeSenden();
  }
  if (millis() - micT >= 200) {                    // Pegel fuers Dashboard (auf 10 Bit skaliert wie vorher)
    micT = millis(); micPP = (micMax - micMin) >> 2; micMitte = micN ? (micSum / micN) >> 2 : 0;
    micMin = 4095; micMax = 0; micSum = 0; micN = 0;
  }
}

// Aufnahme an den Server, Job-Nummer bzw. Fehler ans M5
void stimmeSenden() {
  auto melden = [](const char* thema, const String& t) { audioSenden(thema, (const uint8_t*)t.c_str(), t.length()); };
  if (audioGesamt < 2400) { melden("p1/claude/fehler", "Zu kurz gesprochen - Taste laenger halten"); return; }
  if (p1Token.length() < 24) { melden("p1/claude/fehler", "Kein Server-Zugang: M5 nicht verbunden"); return; }
  melden("p1/claude/start", String(audioGesamt));
  String antwort;
  int code = httpsSenden(P1_STIMME_URL, aufnahme, audioGesamt, antwort);
  int i = antwort.indexOf("\"id\"");
  int a = i >= 0 ? antwort.indexOf('"', antwort.indexOf(':', i) + 1) : -1;
  int e = a >= 0 ? antwort.indexOf('"', a + 1) : -1;
  if (code == 200 && a >= 0 && e > a) {
    String id = antwort.substring(a + 1, e);
    Serial.print("Sprachauftrag "); Serial.println(id);
    melden("p1/claude/job", id);
  } else {
    Serial.print("Sprachauftrag fehlgeschlagen: "); Serial.print(code); Serial.print(" "); Serial.println(antwort.substring(0, 120));
    melden("p1/claude/fehler", "Server-Fehler " + String(code));
  }
}

// Stoerung Kugelstau: Aufzug laeuft, Sensor aktiv, aber zu lange keine Kugel
void stauUpdate() {
  if (!aufzugAn || !SENSOR_AN || STAU_S <= 0) { stau = false; letzteKugelT = millis(); return; }
  if (!stau && millis() - letzteKugelT > (unsigned long)STAU_S * 1000UL) {
    stau = true;
    Serial.print("STOERUNG: Kugelstau, seit "); Serial.print(STAU_S); Serial.println(" s keine Kugel");
  }
}
unsigned long laufSekunden() { return (laufMs + (aufzugAn ? millis() - laufSeit : 0)) / 1000; }

// Eine Kugel freigeben: kurz auf, stoppUpdate macht wieder zu.
// Automatik (manuell = false): nur, wenn der Sensor eine Kugel sieht. Von Hand (Dashboard, 's'): immer.
bool freigeben(bool manuell) {
  if (stoppOffen) return true;
  if (!manuell && SENSOR_AN && !kugelDa) return false;
  if (!SENSOR_AN) kugelZaehler++;                 // ohne Sensor: Freigaben zaehlen
  if (SENSOR_TYP == 1) kugelDa = false;            // Shock: die gemeldete Kugel ist damit weg
  setStopp(true);
  return true;
}

void stoppUpdate() {
  unsigned long dt = millis() - stoppZeit;
  if (stoppOffen) {
    // zu, sobald die Kugel durch ist (mit Sensor) oder spaetestens nach STOPP_OFFEN_MS
    bool durch = SENSOR_AN && SENSOR_TYP == 0 && !kugelDa && kugelWeg > stoppZeit && millis() - kugelWeg >= (unsigned long)STOPP_NACHLAUF_MS;
    if (durch || dt >= (unsigned long)STOPP_OFFEN_MS) setStopp(false);
  } else if (STOPP_AUTOMATIK && aufzugAn && dt >= (unsigned long)STOPP_INTERVALL_MS) {
    if (!freigeben(false)) stoppZeit = millis() - STOPP_INTERVALL_MS + 300;   // keine Kugel: gleich nochmal schauen
  }
}

void status() {
  Serial.print("Aufzug=");   Serial.print(aufzugAn ? "an" : "aus");
  Serial.print(" Weiche=");  Serial.print(weiche.stellung1 ? 1 : 2);
  Serial.print(" Hebel=");   Serial.print(hebel.stellung1 ? 1 : 2);
  Serial.print(" Stopp=");   Serial.print(stoppOffen ? "offen" : "zu");
  Serial.print(" LEDs=");    Serial.println(ledsAn ? "an" : "aus");
}

void tasterRoh() {
  Serial.print("T_L(A5) dig="); Serial.print(digitalRead(TASTER_L));
  Serial.print("  T_M(A4) dig="); Serial.print(digitalRead(TASTER_M));
  Serial.print("  T_R(A3) dig="); Serial.print(digitalRead(TASTER_R));
  Serial.println("   (1 = losgelassen, 0 = gedrueckt)");
}

// alle Analogeingaenge ausgeben (Diagnose)
void scanAnalog() {
  const int pins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
  for (int i = 0; i < 8; i++) {
    Serial.print("A"); Serial.print(i); Serial.print("=");
    Serial.print(analogRead(pins[i])); Serial.print(" ");
  }
  Serial.print("A8=");  Serial.print(analogRead(A8));
  Serial.print(" A9="); Serial.print(analogRead(A9));
  Serial.print(" A10="); Serial.print(analogRead(A10));
  Serial.print(" A11="); Serial.println(analogRead(A11));
  pinMode(TASTER_L, INPUT_PULLUP); pinMode(TASTER_M, INPUT_PULLUP); pinMode(TASTER_R, INPUT_PULLUP);
}

// Servo-Test ('v'): langsam ganz auf und zu fahren, damit man sieht, ob er ueberhaupt reagiert
void servoTest() {
  Serial.println("Servo-Test an D2 und D3: 0 -> 180 -> Ruhestellung");
  for (int w = 0; w <= 180; w += 5) { servoSchreiben(w); delay(25); }
  delay(400);
  for (int w = 180; w >= 0; w -= 5) { servoSchreiben(w); delay(25); }
  delay(400);
  servoWinkel(stoppOffen ? STOPP_AUF : STOPP_ZU);
  Serial.println("Servo-Test fertig. Nichts bewegt? Stecker drehen (Signal an ~2) und 9V-Netzteil pruefen.");
}

// alle Motoren aus (z. B. vor einem Update)
void allesStopp() {
  setAufzug(false);
  motor(WEICHE_A, WEICHE_B, 0); weiche.aktiv = false;
  motor(HEBEL_A, HEBEL_B, 0);   hebel.aktiv = false;
  setStopp(false);
  musikAus();
}

// ---------- Lautsprecher / Weihnachtsmodus ----------
// Das Lied wird im Dashboard hochgeladen (dort aus MP3 umgewandelt) und liegt als
// 8 kHz / 8 Bit / mono Rohdaten im Flash (/fs/lied.raw) - nicht in der Firmware und nicht im Repo.
// Abspielen: Ticker mit 8 kHz setzt das Tastverhaeltnis einer 31-kHz-PWM auf D13.
extern bool otaBereit;        // Flash-Speicher eingebunden (Web.ino)
const char* LIED_DATEI = "/fs/lied.raw";
const int AUDIO_RATE = 8000;
int LAUTSTAERKE = 100;          // 0..100 %
int VERSTAERKUNG = 5;           // 1..10: hebt leise Stellen an (Kompressor), Spitzen werden weich begrenzt
bool weihnacht = false;
uint32_t liedBytes = 0;
mbed::PwmOut* audioPwm = nullptr;
mbed::PwmOut* audioPwm2 = nullptr;
mbed::Ticker audioTick;
FILE* liedDatei = nullptr;
const uint32_t ABUF = 8192;     // ca. 1 s Puffer
volatile uint8_t abuf[ABUF];
volatile uint32_t aLesen = 0, aSchreiben = 0;
volatile float aLaut = 0;
uint8_t aKurve[256];            // Kennlinie: Rohwert -> Ausgang (weiche Begrenzung)
int aKurveFuer = -1;

void kurveBauen() {
  float g = max(1, VERSTAERKUNG), n = tanhf(g);
  for (int i = 0; i < 256; i++) {
    float x = (i - 128) / 128.0f;
    float y = g <= 1 ? x : tanhf(g * x) / n;
    aKurve[i] = (uint8_t)constrain((int)lroundf(128 + y * 127), 0, 255);
  }
  aKurveFuer = VERSTAERKUNG;
}

void audioIsr() {
  float v = 0;
  if (aLesen != aSchreiben) { v = aKurve[abuf[aLesen % ABUF]] / 255.0f * aLaut; aLesen++; }
  audioPwm->write(v); audioPwm2->write(v);
}

void lautsprecherStill() {
  audioTick.detach();
  if (audioPwm) {                       // Timer-Takt fuer evtl. mitbenutzende Motor-Pins zuruecksetzen (analogWrite: 2 ms)
    audioPwm->write(0); audioPwm->period_ms(2);
    delete audioPwm; audioPwm = nullptr;
  }
  if (audioPwm2) { audioPwm2->write(0); audioPwm2->period_ms(2); delete audioPwm2; audioPwm2 = nullptr; }
  pinMode(LAUTSPR_A, OUTPUT); digitalWrite(LAUTSPR_A, LOW);
  pinMode(LAUTSPR_M6, OUTPUT); digitalWrite(LAUTSPR_M6, LOW);
  pinMode(LAUTSPR_M6_GND, OUTPUT); digitalWrite(LAUTSPR_M6_GND, LOW);
}

void musikAus() {
  if (weihnacht) Serial.println("Weihnachtsmodus AUS");
  weihnacht = false;
  lautsprecherStill();
  if (liedDatei) { fclose(liedDatei); liedDatei = nullptr; }
}
bool musikLaeuft() { return weihnacht; }

void liedPruefen() {
  FILE* f = fopen(LIED_DATEI, "rb");
  liedBytes = 0;
  if (f) { fseek(f, 0, SEEK_END); liedBytes = ftell(f); fclose(f); }
}

bool setWeihnacht(bool an) {
  if (!an) { musikAus(); return true; }
  if (!otaBereit || liedBytes == 0) { Serial.println("Weihnachtsmodus: erst ein Lied im Dashboard hochladen"); return false; }
  liedDatei = fopen(LIED_DATEI, "rb");
  if (!liedDatei) return false;
  if (!audioPwm) { audioPwm = new mbed::PwmOut(digitalPinToPinName(LAUTSPR_A)); audioPwm->period_us(32); }
  if (!audioPwm2) { audioPwm2 = new mbed::PwmOut(digitalPinToPinName(LAUTSPR_M6)); audioPwm2->period_us(32); }
  digitalWrite(LAUTSPR_M6_GND, LOW);
  audioPwm->write(0); audioPwm2->write(0);
  kurveBauen();
  aLesen = aSchreiben = 0;
  weihnacht = true;
  musikUpdate();                                   // Puffer vorfuellen
  audioTick.attach(&audioIsr, std::chrono::microseconds(1000000 / AUDIO_RATE));
  Serial.println("Weihnachtsmodus AN");
  return true;
}

// im loop: Puffer aus der Datei nachfuellen, am Ende von vorn
void musikUpdate() {
  if (!weihnacht || !liedDatei) return;
  aLaut = LAUTSTAERKE / 100.0f;
  if (aKurveFuer != VERSTAERKUNG) kurveBauen();
  uint8_t tmp[512];
  while (ABUF - (aSchreiben - aLesen) > sizeof(tmp)) {
    size_t n = fread(tmp, 1, sizeof(tmp), liedDatei);
    if (n == 0) { fseek(liedDatei, 0, SEEK_SET); n = fread(tmp, 1, sizeof(tmp), liedDatei); if (n == 0) return; }
    for (size_t i = 0; i < n; i++) abuf[(aSchreiben + i) % ABUF] = tmp[i];
    aSchreiben += n;
  }
}

// ein Befehl - gemeinsam fuer seriellen Monitor und Weboberflaeche
bool befehl(char c) {
  switch (c) {
    case 'a': setAufzug(!aufzugAn); return true;
    case 'w': umschalten(weiche);   return true;
    case 'h': umschalten(hebel);    return true;
    case 's': freigeben(true);      return true;
    case 'l': setLeds(!ledsAn);     return true;
    case 'x': allesStopp(); notAus = true; Serial.println("NOT-AUS"); return true;
    case 't': tasterRoh();          return true;
    case '?': status(); webStatusSeriell(); return true;
    case 'n': wlanScan();           return true;
    case 'g': githubDiagnose();     return true;
    case 'u': githubUpdateAnfordern(); return true;
    case 'p': scanAnalog();         return true;
    case 'm': live = !live;         return true;
    case 'v': servoTest();          return true;
    case 'j': return setWeihnacht(!weihnacht);
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  motor(AUFZUG_A, AUFZUG_B, 0);
  motor(WEICHE_A, WEICHE_B, 0);
  motor(HEBEL_A, HEBEL_B, 0);
  stopp.attach(SERVO_STOPP);
  stopp2.attach(SERVO_STOPP2);
  servoWinkel(STOPP_ZU);
  setLeds(true);

  // aktuellen Tasterzustand uebernehmen, damit beim Start nichts ausloest
  tasterInit(tL);
  tasterInit(tM);
  tasterInit(tR);
  for (int i = 0; i < N_SENSOR; i++) pinMode(SENSOR_PINS[i], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PINS[0]), stossIsr, CHANGE);
  pinMode(LAUTSPR_A, OUTPUT); lautsprecherStill();

  Serial.println("Parcours 1 bereit.");
  Serial.println("a=Aufzug w=Stellweiche h=Weichenhebel s=Stoppstelle l=LEDs");
  Serial.println("x=alles stoppen t=Taster m=Taster live p=alle Analogpins ?=Status");

  webSetup();
  mqttSetup();
  mikroSetup();
  liedPruefen();
  einstLaden();   // gespeicherte Einstellungen aus dem Flash (braucht den in webSetup eingebundenen Speicher)
}

void loop() {
  if (Serial.available()) befehl(Serial.read());

  if (gedrueckt(tM)) { Serial.println("Taster MITTE"); setAufzug(!aufzugAn); }
  sprechtasteUpdate();
  if (gedrueckt(tR)) { Serial.println("Taster RECHTS"); umschalten(hebel); }

  stellUpdate(weiche);
  stellUpdate(hebel);
  sensorUpdate();
  musikUpdate();
  stoppUpdate();
  servoUpdate();
  ledUpdate();

  if (live && millis() - liveZeit >= 300) { liveZeit = millis(); tasterRoh(); }

  unsigned long t0 = millis();
  webLoop();
  unsigned long t1 = millis();
  mqttLoop();
  unsigned long t2 = millis();
  stauUpdate();
  dWebA = max(dWebA, t1 - t0); dMqttA = max(dMqttA, t2 - t1);
  if (millis() - dT > 3000) { dT = millis(); dWeb = dWebA; dMqtt = dMqttA; dRest = dRestA; dWebA = dMqttA = dRestA = 0; }
}
