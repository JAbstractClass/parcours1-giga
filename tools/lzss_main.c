/* Kommandozeile fuer lzss.c (aus arduino-libraries/ArduinoIoTCloud, public domain)
   Aufruf: lzss --encode|--decode EINGABE AUSGABE */
#include <string.h>
#include "lzss.c"
int main(int argc, char **argv) {
  if (argc != 4) { printf("Aufruf: lzss --encode|--decode in out\n"); return 2; }
  if (!strcmp(argv[1], "--encode")) return encode_file(argv[2], argv[3]);
  if (!strcmp(argv[1], "--decode")) return decode_file(argv[2], argv[3]);
  return 2;
}
