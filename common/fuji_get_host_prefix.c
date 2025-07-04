#include "fujinet-fuji.h"

bool fuji_get_host_prefix(uint8_t hs, char *prefix)
{
  if (!FUJICALL_A1(FUJICMD_GET_HOST_PREFIX, hs))
    return false;

  fuji_bus_get(prefix, 256);
  return true;
}
