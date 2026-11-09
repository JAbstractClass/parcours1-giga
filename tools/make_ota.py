#!/usr/bin/env python3
"""Erzeugt aus einer LZSS-komprimierten Firmware eine .ota-Datei fuer den Arduino Giga.

Format wie bin2ota.py aus arduino-libraries/ArduinoIoTCloud:
  [Laenge 4 B][CRC32 4 B][Magic 0x23410266 4 B][Version 8 B, Kompressions-Flag][Daten]
CRC32 ist der Standard-CRC (zlib), identisch mit crccheck.crc.Crc32.
"""
import sys
import zlib

GIGA_MAGIC = 0x23410266


def main():
    if len(sys.argv) != 3:
        sys.exit("Aufruf: make_ota.py firmware.lzss firmware.ota")
    with open(sys.argv[1], "rb") as f:
        daten = f.read()
    version = bytes([0, 0, 0, 0, 0, 0, 0, 0x40])  # 0x40 = komprimiert
    nutzlast = GIGA_MAGIC.to_bytes(4, "little") + version + daten
    crc = zlib.crc32(nutzlast) & 0xFFFFFFFF
    with open(sys.argv[2], "wb") as f:
        f.write(len(nutzlast).to_bytes(4, "little"))
        f.write(crc.to_bytes(4, "little"))
        f.write(nutzlast)


if __name__ == "__main__":
    main()
