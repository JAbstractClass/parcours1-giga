#pragma once
// Strukturen in eigenem Header, damit die automatischen Arduino-Prototypen sie kennen.

struct Stellmotor {
  int pinA, pinB;
  const char* name;
  bool stellung1;
  unsigned long start;
  bool aktiv;
  int* pwm;                // Kraft 0..255 (zeigt auf die Einstellung)
  int* pulsMs;             // Fahrzeit bis Anschlag
  int* umk;                // 1 = Richtung umkehren
};

// eine im Dashboard einstellbare Groesse
struct Param {
  const char* key;
  int* wert;
  int min, max, def;
};

// Taster: Ruhezustand wird beim Start gemessen, gedrueckt = Abweichung davon
// (funktioniert fuer Schalten nach GND und nach 3,3 V)
struct Taster {
  int pin;
  bool ruhe;             // Pegel im losgelassenen Zustand
  bool zustand;          // entprellter Pegel
  bool roh;
  unsigned long seit;
};
