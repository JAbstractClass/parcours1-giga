#!/bin/zsh
# Firmware per USB-Kabel aufspielen (z. B. Erstinstallation oder Rettung nach kaputtem Sketch).
# Mit Commit-Kennung gebaut -> danach laeuft das Auto-Update von GitHub.
# Ungespeicherte Aenderungen? Dann wird "lokal" gebaut (kein Auto-Update).
set -e
cd "${0:A:h}"
CLI=~/.local/bin/arduino-cli
PORT=${1:-$(ls /dev/cu.usbmodem* 2>/dev/null | head -1)}
[[ -n $PORT ]] || { echo "Kein Giga per USB gefunden"; exit 1; }

if git diff --quiet HEAD -- Parcours1 2>/dev/null; then
  SHA=$(git rev-parse --short=7 HEAD)
else
  SHA=lokal
fi
printf '#pragma once\n#define FW_COMMIT "%s"\n' "$SHA" > Parcours1/version.h
echo "== Baue Firmware $SHA fuer $PORT"
$CLI compile -b arduino:mbed_giga:giga Parcours1 | grep -E "Sketch verwendet|error" || true
$CLI upload -b arduino:mbed_giga:giga -p "$PORT" Parcours1
rm -f Parcours1/version.h
echo "== Fertig"
