#include <fujinet-fuji.h>

/* The 2 ms tick counter, kept by HDOS or by the CP/M BIOS */
#ifdef __HDOS__
#define TICKS (*(volatile uint16_t *) 0x201B)
#else
#define TICKS (*(volatile uint16_t *) 0x000B)
#endif

void wait_jiffies(uint16_t jiffies)
{
  uint16_t start = TICKS;
  uint16_t ticks = (uint16_t) jiffies * 25 / 3;

  while ((uint16_t) (TICKS - start) < ticks)
    ;
}
