#include "fujinet-fuji.h"
#include "fujinet-nes.h"

uint8_t fuji_user_rom_status(FujiUserRomProgress *progress)
{
  uint8_t state;

  progress->percent = 0;
  progress->error = 0;
  progress->got = 0;
  progress->total = 0;

  if (!fuji_nes_present())
    return FUJI_USER_ROM_NO_CART;

  state = fuji_nes_boot_state();
  progress->percent = fuji_nes_boot_percent();
  progress->got = fuji_nes_boot_got();
  progress->total = fuji_nes_boot_total();

  if (state == FUJI_NES_BOOT_READY)
    return FUJI_USER_ROM_READY;
  if (state == FUJI_NES_BOOT_FAILED) {
    progress->error = fuji_nes_boot_error();
    return FUJI_USER_ROM_FAILED;
  }
  return FUJI_USER_ROM_LOADING;
}
