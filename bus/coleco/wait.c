#include <fujinet-fuji.h>

/* Counted, not HALT: the vblank NMI may be off. 590 passes of 101 T-states is about one NTSC frame. */
void fuji_wait_jiffies(uint16_t jiffies)
{
  uint16_t i;

  while (jiffies--) {
    for (i = 0; i < 590; i++) {
      __asm
        nop
      __endasm;
    }
  }
}
