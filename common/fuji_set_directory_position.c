#include <fujinet-fuji.h>

bool fuji_set_directory_position(uint16_t pos)
{
#ifdef _CMOC_VERSION_
  return FUJICALL_A1_A2(FUJICMD_SET_DIRECTORY_POSITION, U16_MSB(pos), U16_LSB(pos));
#else
  return FUJICALL_B12(FUJICMD_SET_DIRECTORY_POSITION, pos);
#endif
}
