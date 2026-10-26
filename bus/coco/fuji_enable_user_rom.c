#include "fujinet-fuji.h"

#define CART_IO_CONTROL ((uint8_t *) 0xFF41)

void fuji_enable_user_rom(void)
{
  *CART_IO_CONTROL = FUJI_CART_ROM_MODE_CMD | FUJI_CART_USER_ROM_ENABLE | FUJI_CART_AUTOSTART;
}
