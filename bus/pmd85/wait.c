#include <fujinet-fuji.h>

/* No timer to rely on, so count cycles. Video memory access slows the CPU so a
 * cycle-counted loop runs about 18% long; 1458 x 24 is within 1% of 1/50 s on
 * hardware and in the emulator. */
static void one_jiffy(void)
{
  __asm
    ld   bc, 1458
  pmd85_wait_loop:
    dec  bc
    ld   a, b
    or   c
    jp   nz, pmd85_wait_loop
  __endasm;
}

void fuji_wait_jiffies(uint16_t jiffies)
{
  while (jiffies--)
    one_jiffy();
}
