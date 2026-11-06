#include <fujinet-fuji.h>

/* Counted, not timed: the vblank NMI may be off. One pass is a jiffy, 1/60 s (29,830 cycles). */
void fuji_wait_jiffies(uint16_t jiffies)
{
  uint16_t i;

  while (jiffies--) {
    for (i = 0; i < 387; i++) {
      __asm__("nop");
    }
  }
}
