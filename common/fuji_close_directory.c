#include <fujinet-fuji.h>

bool fuji_close_directory(void)
{
  return FUJICALL(FUJICMD_CLOSE_DIRECTORY);
}
