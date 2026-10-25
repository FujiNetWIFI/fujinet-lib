#include "harness.h"
#include <time.h>

void wait_jiffies(uint8_t jiffies)
{
  clock_t start = clock();
  clock_t ticks = (clock_t) jiffies * CLOCKS_PER_SEC / 60;

  while (clock() - start < ticks)
    ;
}
