#pragma once
// WLAN-Zugangsdaten. Liegt bewusst im (oeffentlichen) Repo, damit GitHub Actions bauen kann.
// Vorlage fuer andere: arduino_secrets.example.h

// true  = Giga spannt eigenes WLAN auf (Access Point)
// false = Giga verbindet sich mit einem vorhandenen WLAN (z. B. Handy-Hotspot, WPA2 mit Passwort)
//         eduroam geht NICHT (WPA2-Enterprise wird vom Giga nicht unterstuetzt)
#define WLAN_EIGENES_NETZ false

// eigenes WLAN des Giga (Passwort mind. 8 Zeichen)
#define AP_SSID "Parcours1"
#define AP_PASS "gvsijumjVNVV"

// vorhandenes WLAN (nur wenn WLAN_EIGENES_NETZ false)
#define WLAN_SSID "iPhone von mir"
#define WLAN_PASS "abcdefgh"
