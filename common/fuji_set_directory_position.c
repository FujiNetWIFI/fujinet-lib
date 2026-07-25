#include <fujinet-fuji.h>

bool fuji_set_directory_position(uint16_t pos)
{
  return FUJICALL_B12(FUJICMD_SET_DIRECTORY_POSITION, pos);
}
