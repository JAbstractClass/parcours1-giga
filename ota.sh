#!/bin/zsh
# Kompiliert Parcours1, erzeugt die OTA-Datei und stellt sie im WLAN bereit.
# Danach auf der Weboberflaeche des Giga "Update starten" klicken.
# Laptop und Giga muessen im selben WLAN sein (Hotspot).
set -e
cd "${0:A:h}"

CLI=~/.local/bin/arduino-cli
FQBN=arduino:mbed_giga:giga
OUT=build
PORT=8000

[[ -x tools/lzss ]] || cc -O2 -w -o tools/lzss tools/lzss_main.c

echo "== Kompiliere ..."
$CLI compile -b $FQBN --output-dir $OUT Parcours1 | grep -E "Sketch verwendet|error" || true
[[ -f $OUT/Parcours1.ino.bin ]] || { echo "Kompilieren fehlgeschlagen"; exit 1; }

echo "== Erzeuge OTA-Datei ..."
mkdir -p $OUT/ota
tools/lzss --encode $OUT/Parcours1.ino.bin $OUT/Parcours1.lzss >/dev/null
# Gegenprobe: entpacken muss exakt die Firmware ergeben
tools/lzss --decode $OUT/Parcours1.lzss $OUT/check.bin
cmp -s $OUT/Parcours1.ino.bin $OUT/check.bin || { echo "LZSS-Pruefung fehlgeschlagen"; exit 1; }
rm $OUT/check.bin
python3 -I tools/make_ota.py $OUT/Parcours1.lzss $OUT/ota/Parcours1.ota
ls -l $OUT/ota/Parcours1.ota | awk '{print "   " $5 " Bytes"}'

IP=$(ipconfig getifaddr en0 2>/dev/null || true)
echo
echo "== Bereit. Auf der Giga-Seite 'Update starten' klicken."
[[ -n $IP ]] && echo "   Datei: http://$IP:$PORT/Parcours1.ota"
echo "   (Beenden mit Ctrl+C, wenn das Update durch ist)"
echo
python3 -m http.server $PORT --bind 0.0.0.0 --directory $OUT/ota
