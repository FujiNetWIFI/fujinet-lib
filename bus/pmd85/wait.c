#include <fujinet-fuji.h>

/* No timer to rely on, so count cycles: 1420 x 24 + 10 is 34,090, and the call
 * and loop around it bring one jiffy to about 34,133 at 2.048 MHz */
static void one_jiffy(void)
{
  __asm
    ld   bc, 1420
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
