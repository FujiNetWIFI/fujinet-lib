#include <fujinet-fuji.h>

/* The 2 ms tick counter, kept by HDOS or by the CP/M BIOS */
#ifdef __HDOS__
#define TICKS (*(volatile uint16_t *) 0x201B)
#else
#define TICKS (*(volatile uint16_t *) 0x000B)
#endif

static void wait_ticks(uint16_t *start, uint8_t ticks)
{
  while ((uint16_t) (TICKS - *start) < ticks)
    ;
  *start += ticks;
}

void fuji_wait_jiffies(uint16_t jiffies)
{
  uint16_t start = TICKS;

  /* Three jiffies are 25 ticks exactly; stepping start keeps the math 16-bit */
  for (; jiffies >= 3; jiffies -= 3)
    wait_ticks(&start, 25);
  wait_ticks(&start, (uint8_t) jiffies * 25 / 3);
}
