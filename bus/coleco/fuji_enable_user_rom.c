#include "fujinet-fuji.h"
#include "fujinet-coleco.h"

void fuji_enable_user_rom(void)
{
  fuji_coleco_boot_swap();
}
