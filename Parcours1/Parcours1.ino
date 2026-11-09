// Parcours 1 - fischertechnik Dynamic XXL
// Mega-F5 Shield auf Arduino Giga R1 WiFi
// Belegung laut "Verkabelung_Parcours1_Arduino 2.pdf" (inkl. handschriftlicher Taster-Notiz)
//
// Taster: beim Start NICHT druecken - der Ruhezustand wird dann gemessen.
// Bedienung per Taster:
//   T_M (Mitte)  -> Kettenaufzug AN/AUS
//   T_L (links)  -> Stellweiche (Flexschiene) umschalten
//   T_R (rechts) -> Stellweiche mit Weichenhebel umschalten
// Stoppstelle: oeffnet automatisch alle STOPP_INTERVALL_MS kurz, solange der Aufzug laeuft.
//
// Serieller Monitor (115200 Baud):
//   a = Aufzug   w = Stellweiche   h = Weichenhebel   s = Stoppstelle
//   l = LEDs     t = Taster-Rohwerte   ? = Status
//   m = Taster-Rohwerte live an/aus      p = alle Analogpins
//   x = alles stoppen   n = WLANs in Reichweite anzeigen
//
// WLAN, Weboberflaeche und OTA-Update: siehe Web.ino und arduino_secrets.h

#include <Servo.h>
#include "Typen.h"
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
const int SERVO_STOPP = 2;   // SERVO 1

const int TASTER_L = A5;     // T_L
const int TASTER_M = A4;     // T_M
const int TASTER_R = A3;     // T_R

// ---------- Kalibrierung ----------
const int  AUFZUG_PWM         = 200;   // 0..255, Geschwindigkeit Kettenaufzug
const bool AUFZUG_UMKEHREN    = false; // true, falls Aufzug falsch herum laeuft
const int  STELL_PWM          = 180;   // Kraft der Weichenmotoren
const unsigned long STELL_PULS_MS = 350; // Fahrzeit bis Anschlag
const int  STOPP_ZU           = 60;    // Servo-Winkel: Kugel gestoppt
const int  STOPP_AUF          = 120;   // Servo-Winkel: Kugel frei
const bool STOPP_AUTOMATIK    = true;  // Stoppstelle automatisch takten
const unsigned long STOPP_INTERVALL_MS = 8000; // Abstand zwischen Freigaben
const unsigned long STOPP_OFFEN_MS     = 800;  // wie lange offen
const unsigned long ENTPRELL_MS = 40;

// ---------- Zustand ----------
Servo stopp;
bool aufzugAn = false, ledsAn = true, stoppOffen = false;
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
Stellmotor weiche = {WEICHE_A, WEICHE_B, "Weiche", true, 0, false};
Stellmotor hebel  = {HEBEL_A,  HEBEL_B,  "Hebel",  true, 0, false};

void umschalten(Stellmotor& m) {
  if (m.aktiv) return;                 // laeuft noch
  m.stellung1 = !m.stellung1;
  motor(m.pinA, m.pinB, m.stellung1 ? STELL_PWM : -STELL_PWM);
  m.start = millis();
  m.aktiv = true;
  Serial.print(m.name);
  Serial.println(m.stellung1 ? ": Stellung 1" : ": Stellung 2");
}

void stellUpdate(Stellmotor& m) {
  if (m.aktiv && millis() - m.start >= STELL_PULS_MS) {
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
  aufzugAn = an;
  int v = an ? AUFZUG_PWM : 0;
  motor(AUFZUG_A, AUFZUG_B, AUFZUG_UMKEHREN ? -v : v);
  stoppZeit = millis();
  Serial.println(an ? "Aufzug AN" : "Aufzug AUS");
}

void setLeds(bool an) {
  ledsAn = an;
  digitalWrite(LED_1, an ? HIGH : LOW);
  digitalWrite(LED_2, an ? HIGH : LOW);
  Serial.println(an ? "LEDs AN" : "LEDs AUS");
}

void setStopp(bool offen) {
  stoppOffen = offen;
  stopp.write(offen ? STOPP_AUF : STOPP_ZU);
  stoppZeit = millis();
  Serial.println(offen ? "Stoppstelle offen" : "Stoppstelle zu");
}

void stoppUpdate() {
  if (!STOPP_AUTOMATIK || !aufzugAn) return;
  unsigned long dt = millis() - stoppZeit;
  if (stoppOffen && dt >= STOPP_OFFEN_MS) setStopp(false);
  else if (!stoppOffen && dt >= STOPP_INTERVALL_MS) setStopp(true);
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

// alle Motoren aus (z. B. vor einem Update)
void allesStopp() {
  setAufzug(false);
  motor(WEICHE_A, WEICHE_B, 0); weiche.aktiv = false;
  motor(HEBEL_A, HEBEL_B, 0);   hebel.aktiv = false;
  setStopp(false);
}

// ein Befehl - gemeinsam fuer seriellen Monitor und Weboberflaeche
bool befehl(char c) {
  switch (c) {
    case 'a': setAufzug(!aufzugAn); return true;
    case 'w': umschalten(weiche);   return true;
    case 'h': umschalten(hebel);    return true;
    case 's': setStopp(!stoppOffen); return true;
    case 'l': setLeds(!ledsAn);     return true;
    case 'x': allesStopp();         return true;
    case 't': tasterRoh();          return true;
    case '?': status(); webStatusSeriell(); return true;
    case 'n': wlanScan();           return true;
    case 'p': scanAnalog();         return true;
    case 'm': live = !live;         return true;
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
  stopp.write(STOPP_ZU);
  setLeds(true);

  // aktuellen Tasterzustand uebernehmen, damit beim Start nichts ausloest
  tasterInit(tL);
  tasterInit(tM);
  tasterInit(tR);

  Serial.println("Parcours 1 bereit.");
  Serial.println("a=Aufzug w=Stellweiche h=Weichenhebel s=Stoppstelle l=LEDs");
  Serial.println("x=alles stoppen t=Taster m=Taster live p=alle Analogpins ?=Status");

  webSetup();
}

void loop() {
  if (Serial.available()) befehl(Serial.read());

  if (gedrueckt(tM)) { Serial.println("Taster MITTE"); setAufzug(!aufzugAn); }
  if (gedrueckt(tL)) { Serial.println("Taster LINKS"); umschalten(weiche); }
  if (gedrueckt(tR)) { Serial.println("Taster RECHTS"); umschalten(hebel); }

  stellUpdate(weiche);
  stellUpdate(hebel);
  stoppUpdate();

  if (live && millis() - liveZeit >= 300) { liveZeit = millis(); tasterRoh(); }

  webLoop();
}
