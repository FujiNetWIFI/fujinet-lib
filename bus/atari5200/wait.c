#include <fujinet-fuji.h>

/* The BIOS's VBI counts RTCLOK, as the OS does on the Atari */
#define RTCLOK_LOW (*(volatile uint8_t *) 0x02)

void fuji_wait_jiffies(uint16_t jiffies)
{
  uint8_t last = RTCLOK_LOW;

  /* Count the low byte's ticks one at a time, so a wait can span its wrap */
  while (jiffies) {
    if (RTCLOK_LOW != last) {
      ++last;
      --jiffies;
    }
  }
}
