#include <fujinet-fuji.h>

/* The 2 ms tick counter, kept by HDOS or by the CP/M BIOS */
#ifdef __HDOS__
#define TICKS (*(volatile uint16_t *) 0x201B)
#else
#define TICKS (*(volatile uint16_t *) 0x000B)
#endif

static void wait_until(uint16_t target)
{
  while ((int16_t) (TICKS - target) < 0)
    ;
}

void wait_jiffies(uint16_t jiffies)
{
  uint16_t target = TICKS;

  /* Three jiffies are 25 ticks exactly; stepping the target keeps the math 16-bit */
  for (; jiffies >= 3; jiffies -= 3)
    wait_until(target += 25);
  wait_until(target + (uint8_t) jiffies * 25 / 3);
}
