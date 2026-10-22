#include "fujinet-fuji.h"
#include <string.h>

#define CART_IO_CONTROL 0xBFFF
#define STUB_ADDR       0xC000

/* The write switches out the ROM the caller may be running from, so it runs
 * from RAM. */
static const uint8_t user_rom_stub[] = {
  0x21, CART_IO_CONTROL & 0xFF, CART_IO_CONTROL >> 8,          /* LD HL,CART_IO_CONTROL */
  0x36, FUJI_CART_ROM_MODE_CMD | FUJI_CART_USER_ROM_ENABLE,    /* LD (HL),flags */
  0xC3, 0x00, 0x00                                             /* JP 0 */
};

void fuji_enable_user_rom(void)
{
  memcpy((void *) STUB_ADDR, user_rom_stub, sizeof(user_rom_stub));
  __asm
    jp STUB_ADDR
  __endasm;
}
