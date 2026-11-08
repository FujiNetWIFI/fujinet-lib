#include "fujinet-fuji.h"
#include "fujinet-coleco.h"

uint8_t fuji_user_rom_status(FujiUserRomProgress *progress)
{
  uint8_t state;

  progress->percent = 0;
  progress->error = 0;
  progress->got = 0;
  progress->total = 0;

  if (!fuji_coleco_present())
    return FUJI_USER_ROM_NO_CART;

  state = fuji_coleco_boot_state();
  progress->percent = fuji_coleco_boot_percent();

  if (state == FUJI_COLECO_BOOT_READY)
    return FUJI_USER_ROM_READY;
  if (state == FUJI_COLECO_BOOT_FAILED) {
    progress->error = fuji_coleco_boot_error();
    return FUJI_USER_ROM_FAILED;
  }
  return FUJI_USER_ROM_LOADING;
}
