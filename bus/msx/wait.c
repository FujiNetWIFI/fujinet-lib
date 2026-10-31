#include <fujinet-fuji.h>

/* One HALT per display frame: 60 Hz, or 50 Hz on PAL machines */
void wait_jiffies(uint8_t jiffies)
{
  while (jiffies--) {
    __asm
      halt
    __endasm;
  }
}
