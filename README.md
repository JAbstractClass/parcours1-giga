# Parcours 1 – fischertechnik Dynamic XXL am Arduino Giga

Arduino Giga R1 WiFi mit Mega-F5 Shield steuert die Kugelbahn: Kettenaufzug, zwei Stellweichen, Stoppstelle (Servo), zwei Rainbow-LEDs, drei Taster.

## So arbeiten wir

1. Code in `Parcours1/` ändern, committen, auf `main` pushen.
2. GitHub Actions baut die Firmware und legt sie auf GitHub Pages: <https://jabstractclass.github.io/parcours1-giga/>
3. Der Giga schaut jede Minute nach, ob es eine neue Version gibt. Wenn ja: Motoren aus, Download, Neustart mit der neuen Firmware (dauert ca. 30 s).

Ob der Build durchging: Tab **Actions** im Repo. Ist er rot, bleibt der Giga auf der alten Version.

## Bedienung

- **Weboberfläche:** Laptop/Handy in den Hotspot „iPhone von mir“, dann die IP aus dem seriellen Monitor im Browser öffnen (bisher `http://172.20.10.2`). Mehrere Geräte gleichzeitig gehen.
- **Taster:** Mitte (A4) = Aufzug an/aus, links (A5) = Stellweiche, rechts (A3) = Weichenhebel. Beim Einschalten nicht drücken.
- **Serieller Monitor** (115200 Baud, `./monitor.command`): `a` Aufzug, `w` Weiche, `h` Hebel, `s` Stoppstelle, `l` LEDs, `x` alles stoppen, `?` Status, `t`/`m` Taster, `n` WLANs anzeigen, `g` GitHub-Verbindung testen, `u` Update von GitHub sofort laden.

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

Taster werden digital mit internem Pull-up gelesen: Die Analogwerte zeigen mit dem Shield an allen Pins dauerhaft 1023.

## Wichtig

- **Jede Firmware muss `Web.ino` mit dem OTA-Teil enthalten**, sonst geht das nächste Update nur noch per USB.
- **Kaputter Sketch / Giga hängt:** RST doppelt drücken (Bootloader), dann per USB: `./flash.sh`
- Lokal per USB mit ungespeicherten Änderungen gebaut → Firmware heißt `lokal` und macht **kein** Auto-Update (damit lokale Tests nicht überschrieben werden). Über den Knopf „Jetzt von GitHub laden“ geht es trotzdem.
- WLAN-Daten stehen in `Parcours1/arduino_secrets.h` (öffentlich im Repo, bewusst so entschieden). eduroam geht mit dem Giga nicht (WPA2-Enterprise).
- Motoren brauchen das 9 V-Netzteil, USB allein reicht nicht.
