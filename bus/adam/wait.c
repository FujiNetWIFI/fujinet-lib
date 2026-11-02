#include <fujinet-fuji.h>

/* The VDP vblank is the Z80's NMI, and the crt's handler reads the VDP
 * status each frame, so one HALT is one 60 Hz frame on any Adam */
void fuji_wait_jiffies(uint16_t jiffies)
{
  while (jiffies--) {
    __asm
      halt
    __endasm;
  }
}
