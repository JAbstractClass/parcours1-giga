#!/bin/zsh
echo "Serieller Monitor Giga – Befehle eintippen + Enter (z.B. m). Beenden: Ctrl+C"
~/.local/bin/arduino-cli monitor -p /dev/cu.usbmodem3101 -c baudrate=115200
