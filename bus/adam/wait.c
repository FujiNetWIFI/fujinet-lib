#include <fujinet-fuji.h>

/* No timer to rely on, so count cycles: 2292 x 26 + 5 is 59,597, and the call
 * and loop around it bring one jiffy to about 59,659 at 3.579545 MHz */
static void one_jiffy(void)
{
  __asm
    ld   bc, 2292
  adam_wait_loop:
    dec  bc
    ld   a, b
    or   c
    jr   nz, adam_wait_loop
  __endasm;
}

void wait_jiffies(uint16_t jiffies)
{
  while (jiffies--)
    one_jiffy();
}
