# Parcours 1 – fischertechnik Dynamic XXL am Arduino Giga

Arduino Giga R1 WiFi mit Mega-F5 Shield steuert die Kugelbahn: Kettenaufzug, zwei Stellweichen, Stoppstelle (Servo), zwei Rainbow-LEDs, drei Taster.

## So arbeiten wir

1. Code in `Parcours1/` ändern, committen, auf `main` pushen.
2. GitHub Actions baut die Firmware und legt sie auf GitHub Pages: <https://jabstractclass.github.io/parcours1-giga/>
3. Der Giga schaut jede Minute nach, ob es eine neue Version gibt. Wenn ja: Motoren aus, Download, Neustart mit der neuen Firmware (dauert ca. 30 s).

Ob der Build durchging: Tab **Actions** im Repo. Ist er rot, bleibt der Giga auf der alten Version.

## Bedienung

- **Weboberfläche:** Laptop/Handy in den Hotspot „iPhone von mir“, dann die IP aus dem seriellen Monitor im Browser öffnen (bisher `http://172.20.10.2`). Mehrere Geräte gleichzeitig gehen.
- **Taster:** Mitte (A4) = Aufzug an/aus, links (A5) = **Sprechtaste** (Mikrofon an A7), rechts (A3) = Weichenhebel. Beim Einschalten nicht drücken.
- **Serieller Monitor** (115200 Baud, `./monitor.command`): `a` Aufzug, `w` Weiche, `h` Hebel, `s` Stoppstelle, `l` LEDs, `x` alles stoppen, `v` Servo-Test, `s` gibt eine Kugel frei (Stoppstelle kurz auf, dann zu), `?` Status, `t`/`m` Taster, `n` WLANs anzeigen, `g` GitHub-Verbindung testen, `u` Update von GitHub sofort laden.

## Aktoren einstellen

Im Dashboard oben rechts **Einstellen**: Geschwindigkeit des Aufzugs, Kraft und Stellweg beider Weichen, Winkel, Offen-Zeit, Tempo und Takt der Stoppstelle, Richtung umkehren, Blinkcode. Jeder Aktor hat einen Test-Knopf. Werte gelten sofort und werden auf dem Giga in `/fs/einst.txt` gespeichert (bleiben auch nach Neustart und Update). „Alles auf Startwerte zurücksetzen“ nimmt wieder die Werte aus dem Block *Kalibrierung* in `Parcours1.ino`.

## Kugelsensor und Weihnachtsmodus

- Sensorart unter Einstellen → Stoppstelle wählen. **Shock-Sensor:** meldet den Aufprall der ankommenden Kugel (per Interrupt, auch sehr kurze Impulse), zählt sie und merkt sich „Kugel wartet“ bis zur nächsten Freigabe; „Mindestabstand“ blendet das Nachschwingen der Feder aus. **Reflex-Sensor:** Der Sensor schaut auf die Kugel, die vor der geschlossenen Stoppstelle wartet. Die Automatik öffnet die Stoppstelle nur, wenn er eine Kugel sieht, und schließt, sobald sie durchgerollt ist. Von Hand (Dashboard, Taste `s`) öffnet sie immer. Jede durchgerollte Kugel erhöht den Zähler („Kugeln durch“). Empfindlichkeit am Poti des Moduls einstellen. Ohne Sensor: unter Einstellen → Stoppstelle „Nur freigeben, wenn der Sensor eine Kugel sieht“ ausschalten.
- Weihnachtsmodus: Im Dashboard ein Lied (MP3) hochladen. Der Browser wandelt es in 8 kHz/8 Bit um und schickt es in den Flash des Giga (`/fs/lied.raw`). Das Lied liegt **nicht** im Repo. Lautstärke unter Einstellen → Lautsprecher.

## M5Stack-Bedienpanel

`M5Panel/` läuft auf einem M5Stack Core (3 Tasten). Das M5Stack ist selbst der MQTT-Broker und kündigt sich im Hotspot per UDP an, der Giga verbindet sich von allein, ganz ohne Internet. Webinterface und M5 zeigen denselben Zustand, Befehle von beiden Seiten gehen an den Giga.

- **Tasten Startseite:** A Menü · B Start/Stopp (lang halten = NOT-AUS) · C Kugel freigeben
- **In Listen:** A Zurück · B Weiter · C OK. **Werte ändern:** A − · B + (halten = schnell) · C Speichern, A lang = abbrechen
- **Menü:** Status aller Ein-/Ausgänge, Manuell steuern, Betriebsparameter, Messwerte & Statistik, Info
- **Sicherheit:** Das M5 schickt alle 400 ms ein Lebenszeichen. Bleibt es 2,5 s aus, stoppt der Giga alles (Aufzug aus, Weichen stromlos, Stoppstelle zu) und meldet die Störung, bis die Anlage wieder gestartet wird. Abschaltbar unter Einstellen → Überwachung. Das M5 piept und zeigt eine rote Warnung, wenn es keine Daten mehr vom Giga bekommt.
- **Kugelstau:** Läuft der Aufzug länger als 30 s (einstellbar), ohne dass eine Kugel ankommt, meldet der Giga eine Störung; das M5 piept.
- **Aufspielen** (per USB, M5Stack am Laptop): `M5Panel/secrets.example.h` nach `M5Panel/secrets.h` kopieren und WLAN eintragen (nicht im Repo), dann
  `arduino-cli compile -b esp32:esp32:m5stack_core:FlashSize=16M,PartitionScheme=huge_app M5Panel` und `arduino-cli upload … -p /dev/cu.usbserial-* M5Panel`.
  Bibliotheken: M5Unified 0.2.2, PicoMQTT 1.3.0, ArduinoJson 7.2.1 (ESP32-Kern 3.0.7).

## Sprachsteuerung mit Claude

1. **Linken Taster am Giga gedrückt halten** und ins Mikrofon (A7) sprechen, dann loslassen. Der Giga liest das Mikrofon per DMA mit 8 kHz (Arduino_AdvancedAnalog) und schickt die Aufnahme per MQTT ans M5Stack.
2. Das M5 wechselt auf die **Claude-Seite**, streamt die Aufnahme an den Server (`https://minime.love66.app/p1`, Dienst `jl-parcours`) und zeigt Claudes Zwischenschritte live.
3. Der Server macht Sprache zu Text (Whisper über `jl-minime`), startet Claude Code in einer Kopie dieses Repos auf einem eigenen Branch `stimme/<id>` und liest die Antwort per edge-tts vor (Lautsprecher des M5).
4. Hat Claude etwas geändert: am M5 **C = übernehmen** (Branch → `main` → GitHub baut → der Giga holt sich die Firmware), **A = verwerfen**. B wiederholt die Antwort bzw. schaltet den Ton aus.

Server-Code: `server/jl-parcours/` (läuft als `jlclaude`, Konfiguration `/etc/jl-parcours.env` mit `P1_TOKEN`, `MINIME_TOKEN`, `CLAUDE_ACCOUNT`). Der Token steht für das M5 in `M5Panel/secrets.h` (nicht im Repo). Claude kann auf dem Server nicht kompilieren; schlägt der Build bei GitHub fehl, bleibt der Giga auf der alten Version. Mikrofon-Verstärkung: Einstellung `mic_gain`.

## Dashboard bearbeiten

Quelle ist `Parcours1/dashboard.html`. Vorschau ohne Giga (simuliert alle Funktionen):

```
python3 tools/dashboard_preview.py     # dann http://localhost:8080
```

Beim Bauen wird daraus automatisch `Parcours1/Dashboard.h` erzeugt (`tools/html2h.py`), nicht von Hand ändern.

## Belegung

| Bauteil | Shield | Giga-Pins |
|---|---|---|
| Kettenaufzug (XS-Motor) | M2 | D4/D5 |
| Stellweiche Flexschiene | M3 | D6/D7 |
| Stellweiche mit Weichenhebel | M4 | D8/D9 |
| Stoppstelle (Servo) | SERVO 1 | D2 |
| Rainbow-LED 1 / 2 | M1 Klemme 16 / 17 → GND | D16 / D17 |
| Taster links / Mitte / rechts | A5 / A4 / A3 → GND | |
| Kugelsensor (Shock B23 oder IR-Reflex B04) | VCC → **3,3 V** (nie 5 V), GND → GND, OUT → IN A6 | A6 |
| Lautsprecher (2 Pins) | leise: Pin 1 → **P15** über **mind. 470 Ω**, Pin 2 → GND · laut: an **M6** über ca. 100 Ω (beide spielen gleichzeitig) | D15 · D12/D13 |
| Statuslampe RGB-Modul (B09, optional) | R → A0/D18, G → A1/D19, B → A2, „–“ → GND | |

Die Stoppstelle ist kein Motor an einem M-Ausgang, sondern ein Servo am 3-poligen Pfostenstecker SERVO 1 (Signal an ~2, VCC, GND) und wird über die Servo-Bibliothek mit Winkeln gestellt (`STOPP_ZU` / `STOPP_AUF`).

## Statusanzeige

| Zustand | RGB-LED auf dem Giga | Rainbow-LEDs | Dashboard |
|---|---|---|---|
| Alles ok, Internet da | grün | Dauerlicht | grün |
| Kein Internet / kein WLAN | orange | blinken langsam (1 s) | orange |
| Not-Aus oder Fehler (z. B. Update fehlgeschlagen) | rot | blinken schnell | rot |

Echte Farben oben am Turm: RGB-LED-Modul aus dem Sensorkit (B09) anschließen, R → IN A0 oder D18, G → IN A1 oder D19, B → IN A2, „-“ → GND (beide Varianten werden gleichzeitig angesteuert; im Dashboard unter System → Statuslampe testbar). Orange ist dort Rot + Grün und wirkt eher gelb. Die Rainbow-LEDs wechseln ihre Farbe selbst und hängen nur an/aus am Shield, deshalb zeigen sie den Zustand als Blinkcode. Not-Aus bleibt aktiv, bis der Aufzug wieder gestartet wird. Weichenkraft und Stellweg je Weiche: `WEICHE_PWM`/`WEICHE_PULS_MS` und `HEBEL_PWM`/`HEBEL_PULS_MS` in `Parcours1.ino`.

Taster werden digital mit internem Pull-up gelesen: Die Analogwerte zeigen mit dem Shield an allen Pins dauerhaft 1023.

## Wichtig

- **Jede Firmware muss `Web.ino` mit dem OTA-Teil enthalten**, sonst geht das nächste Update nur noch per USB.
- **Kaputter Sketch / Giga hängt:** RST doppelt drücken (Bootloader), dann per USB: `./flash.sh`
- Lokal per USB mit ungespeicherten Änderungen gebaut → Firmware heißt `lokal` und macht **kein** Auto-Update (damit lokale Tests nicht überschrieben werden). Über den Knopf „Jetzt von GitHub laden“ geht es trotzdem.
- WLAN-Daten stehen in `Parcours1/arduino_secrets.h` (öffentlich im Repo, bewusst so entschieden). eduroam geht mit dem Giga nicht (WPA2-Enterprise).
- Motoren brauchen das 9 V-Netzteil, USB allein reicht nicht.
- Datenvolumen: Der Giga fragt jede Minute per HTTPS die Version ab, ein Update lädt ca. 380 KB. Das Dashboard zeigt eine Schätzung („Daten des Giga“). Den Gesamtverbrauch des iPhones kennt nur das iPhone selbst (Einstellungen → Mobilfunk). Abfrage-Intervall: `UPDATE_PRUEF_MS` in `Web.ino`.
