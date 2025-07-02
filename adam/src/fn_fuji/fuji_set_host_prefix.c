#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include <eos.h>
#include <string.h>

bool fuji_set_host_prefix(uint8_t hs, char *prefix)
{
  struct _shp
  {
    unsigned char cmd;
    unsigned char hs;
    unsigned char prefix[256];
  } shp;

  shp.cmd = FUJICMD_SET_HOST_PREFIX;
  shp.hs = hs;
  strcpy(shp.prefix,prefix);
  
  eos_write_character_device(0x0f,&shp,sizeof(shp));

  return true;
}
