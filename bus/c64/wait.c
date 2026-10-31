#include <fujinet-fuji.h>

/* KERNAL jiffy clock, 60 Hz on PAL and NTSC */
#define JIFFY_LOW (*(volatile uint8_t *) 0xA2)

void wait_jiffies(uint16_t jiffies)
{
  uint16_t start = JIFFY_LOW;

  while ((uint16_t) (JIFFY_LOW - start) < jiffies)
    ;
}
