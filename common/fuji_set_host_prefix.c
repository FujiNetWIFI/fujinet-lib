#include "fujinet-bus.h"
#include <string.h>

struct _shp
{
  uint8_t hs;
  char prefix[256];
} shp;

bool fuji_set_host_prefix(uint8_t hs, const char *prefix)
{
  shp.hs = hs;
  strcpy(shp.prefix, prefix);

  return FUJICALL_D(FUJICMD_SET_HOST_PREFIX, &shp, sizeof(shp));
}
