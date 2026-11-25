#include <fujinet-fuji.h>
#include "portio.h"

// A jiffy is a sixtieth of a second: 50 ms for every three.
void fuji_wait_jiffies(uint16_t jiffies)
{
  uint16_t start = port_ticks();

  for (; jiffies >= 3; jiffies -= 3) {
    while ((uint16_t) (port_ticks() - start) < 50)
      ;
    start += 50;
  }
  while ((uint16_t) (port_ticks() - start) < (uint16_t) (jiffies * 50 / 3))
    ;
}
