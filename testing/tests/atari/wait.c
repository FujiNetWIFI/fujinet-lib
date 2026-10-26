#include "harness.h"

#define RTCLOK_LOW (*(volatile uint8_t *) 0x14)

void wait_jiffies(uint8_t jiffies)
{
  uint8_t start = RTCLOK_LOW;

  while ((uint8_t) (RTCLOK_LOW - start) < jiffies)
    ;
}
