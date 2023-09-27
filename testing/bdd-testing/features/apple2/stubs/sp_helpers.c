#include <stdint.h>
#include <peekpoke.h>

// creates fake SP at index s (1-7)
void setup_spC(int s) {
  uint16_t a = 0xc000 + (s * 0x100);
  POKE(a+1, 0x20);
  POKE(a+3, 0x00);
  POKE(a+5, 0x03);
  POKE(a+7, 0x00);
}