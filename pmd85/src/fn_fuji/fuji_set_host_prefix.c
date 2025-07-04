#include <dw.h>
#include <fujinet-fuji-pmd85.h>
#include <string.h>
#include "fujinet-fuji.h"

bool fuji_set_host_prefix(uint8_t hs, char *prefix)
{
  struct _shp
  {
    uint8_t opcode;
    uint8_t cmd;
    uint8_t hs;
    char filename[256];    
  } shp;

  shp.opcode = OP_FUJI;
  shp.cmd = FUJICMD_SET_HOST_PREFIX;
  shp.hs = hs;
  strcpy(shp.filename,prefix);

  bus_ready();
  dwwrite((uint8_t *)&shp, sizeof(shp));

  return !fuji_get_error();
}
