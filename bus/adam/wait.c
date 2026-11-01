#include <fujinet-fuji.h>

/* No timer to rely on, so count cycles: 18 x 3346 is about 1/60 s on the 3.58 MHz Z80 */
static void one_jiffy(void)
{
  __asm
    ld   c, 18
  adam_wait_outer:
    ld   b, 0
  adam_wait_inner:
    djnz adam_wait_inner
    dec  c
    jr   nz, adam_wait_outer
  __endasm;
}

void wait_jiffies(uint16_t jiffies)
{
  while (jiffies--)
    one_jiffy();
}
