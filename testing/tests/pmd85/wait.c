#include "harness.h"

/* No timer to rely on, so count cycles: 9 x 3862 is about 1/60 s on the 2.048 MHz 8080 */
static void one_jiffy(void)
{
  __asm
    ld   c, 9
  pmd85_wait_outer:
    ld   b, 0
  pmd85_wait_inner:
    dec  b
    jp   nz, pmd85_wait_inner
    dec  c
    jp   nz, pmd85_wait_outer
  __endasm;
}

void wait_jiffies(uint8_t jiffies)
{
  while (jiffies--)
    one_jiffy();
}
