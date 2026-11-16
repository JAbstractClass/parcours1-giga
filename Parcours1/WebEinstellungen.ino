// ---------- Einstellungen: im Dashboard aenderbar, im Flash gespeichert ----------
// (Dateiname beginnt mit "Web", damit Arduino sie nach Web.ino einfuegt: nutzt otaBereit von dort)
// Startwerte stehen in Parcours1.ino (Kalibrierung). Sobald im Dashboard etwas geaendert wird, speichert der Giga
// alle Werte; "Zuruecksetzen" loescht die Datei und nimmt wieder die Startwerte.

Param PARAMS[] = {
  {"aufzug_pwm",  &AUFZUG_PWM,        0,   255,   0},
  {"aufzug_umk",  &AUFZUG_UMKEHREN,   0,   1,     0},
  {"weiche_pwm",  &WEICHE_PWM,        0,   255,   0},
  {"weiche_ms",   &WEICHE_PULS_MS,    20,  3000,  0},
  {"weiche_umk",  &WEICHE_UMKEHREN,   0,   1,     0},
  {"hebel_pwm",   &HEBEL_PWM,         0,   255,   0},
  {"hebel_ms",    &HEBEL_PULS_MS,     20,  3000,  0},
  {"hebel_umk",   &HEBEL_UMKEHREN,    0,   1,     0},
  {"stopp_zu",    &STOPP_ZU,          0,   180,   0},
  {"stopp_auf",   &STOPP_AUF,         0,   180,   0},
  {"stopp_ms",    &STOPP_OFFEN_MS,    30,  3000,  0},
  {"stopp_tempo", &STOPP_TEMPO,       0,   30,    0},
  {"stopp_auto",  &STOPP_AUTOMATIK,   0,   1,     0},
  {"stopp_takt",  &STOPP_INTERVALL_MS,1000,60000, 0},
  {"led_blink",   &LED_BLINKCODE,     0,   1,     0},
  {"sensor_an",   &SENSOR_AN,         0,   1,     0},
  {"sensor_inv",  &SENSOR_INV,        0,   1,     0},
  {"stopp_nach",  &STOPP_NACHLAUF_MS, 0,   1000,  0},
  {"laut",        &LAUTSTAERKE,       0,   100,   0},
};
const int N_PARAMS = sizeof(PARAMS) / sizeof(PARAMS[0]);
const char* EINST_DATEI = "/fs/einst.txt";
bool einstGeladen = false;

// Aenderungen sofort wirken lassen (laufender Aufzug, Stellung der Stoppstelle)
void einstAnwenden() {
  if (aufzugAn) { int v = AUFZUG_PWM; motor(AUFZUG_A, AUFZUG_B, AUFZUG_UMKEHREN ? -v : v); }
  servoWinkel(stoppOffen ? STOPP_AUF : STOPP_ZU);
}

void einstSpeichern() {
  if (!otaBereit) return;          // Flash nicht eingebunden -> Werte gelten bis zum Neustart
  FILE* f = fopen(EINST_DATEI, "w");
  if (!f) { Serial.println("Einstellungen: Datei nicht schreibbar"); return; }
  for (int i = 0; i < N_PARAMS; i++) fprintf(f, "%s=%d\n", PARAMS[i].key, *PARAMS[i].wert);
  fclose(f);
}

bool einstSetzen(const String& k, int v) {
  for (int i = 0; i < N_PARAMS; i++) {
    if (k == PARAMS[i].key) { *PARAMS[i].wert = constrain(v, PARAMS[i].min, PARAMS[i].max); return true; }
  }
  return false;
}

void einstLaden() {
  for (int i = 0; i < N_PARAMS; i++) PARAMS[i].def = *PARAMS[i].wert;   // Startwerte merken (fuer "Zuruecksetzen")
  einstGeladen = true;
  otaVorbereiten();
  if (!otaBereit) { Serial.println("Einstellungen: kein Flash, nutze Startwerte"); return; }
  FILE* f = fopen(EINST_DATEI, "r");
  if (!f) { Serial.println("Einstellungen: Startwerte"); return; }
  char z[64]; int n = 0;
  while (fgets(z, sizeof(z), f)) {
    String s = String(z); s.trim();
    int e = s.indexOf('=');
    if (e > 0 && einstSetzen(s.substring(0, e), s.substring(e + 1).toInt())) n++;
  }
  fclose(f);
  Serial.print("Einstellungen geladen: "); Serial.println(n);
  einstAnwenden();
}

void einstZuruecksetzen() {
  for (int i = 0; i < N_PARAMS; i++) *PARAMS[i].wert = PARAMS[i].def;
  if (otaBereit) remove(EINST_DATEI);
  einstAnwenden();
}

String einstJson() {
  String s = "{";
  for (int i = 0; i < N_PARAMS; i++) {
    if (i) s += ",";
    s += "\""; s += PARAMS[i].key; s += "\":["; s += *PARAMS[i].wert; s += ","; s += PARAMS[i].def; s += "]";
  }
  s += ",\"gespeichert\":"; s += otaBereit ? "true" : "false";
  return s + "}";
}
