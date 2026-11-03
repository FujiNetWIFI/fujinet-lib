#include <fujinet-fuji.h>

/* Counted, not HALT: a program may run with the vblank NMI off, and HALT
 * would then never wake. About one NTSC frame (59659 T-states) per jiffy. */
void fuji_wait_jiffies(uint16_t jiffies)
{
  uint16_t i;

  while (jiffies--) {
    for (i = 0; i < 2300; i++) {
      __asm
        nop
      __endasm;
    }
  }
}
