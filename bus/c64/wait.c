#include <fujinet-fuji.h>

/* KERNAL jiffy clock, 60 Hz on PAL and NTSC */
#define JIFFY_LOW (*(volatile uint8_t *) 0xA2)

void fuji_wait_jiffies(uint16_t jiffies)
{
  uint8_t last = JIFFY_LOW;

  /* Count the low byte's ticks one at a time, so a wait can span its wrap */
  while (jiffies) {
    if (JIFFY_LOW != last) {
      ++last;
      --jiffies;
    }
  }
}
