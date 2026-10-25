#include "harness.h"

/* P&T CP/M's clock, 1/100 s since boot (special function 14) */
static uint16_t ticks_100hz(void) __naked
{
  __asm
    ld   b, 14
    call 0x40       ; DE = low word, HL = high word
    ex   de, hl
    ret
  __endasm;
}

void wait_jiffies(uint8_t jiffies)
{
  uint16_t start = ticks_100hz();
  uint16_t ticks = (uint16_t) jiffies * 5 / 3;

  while ((uint16_t) (ticks_100hz() - start) < ticks)
    ;
}
