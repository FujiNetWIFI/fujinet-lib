#include <fujinet-fuji.h>

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

static void wait_until(uint16_t target)
{
  while ((int16_t) (ticks_100hz() - target) < 0)
    ;
}

void wait_jiffies(uint16_t jiffies)
{
  uint16_t target = ticks_100hz();

  /* Three jiffies are 5 ticks exactly; stepping the target keeps the math 16-bit */
  for (; jiffies >= 3; jiffies -= 3)
    wait_until(target += 5);
  wait_until(target + (uint8_t) jiffies * 5 / 3);
}
