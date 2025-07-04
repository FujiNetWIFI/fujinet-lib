#include "fujinet-fuji.h"

bool fuji_get_host_prefix(uint8_t hs, char *prefix)
{
  struct _ghp
  {
    uint8_t opcode;
    uint8_t cmd;
    uint8_t hs;
  } ghp;

  ghp.opcode = OP_FUJI;
  ghp.cmd = FUJICMD_GET_HOST_PREFIX;
  ghp.hs = hs;

  bus_ready();

  dwwrite((uint8_t *)&ghp, sizeof(ghp));
  if (fuji_get_error())
    return false;
  
  return fuji_get_response((uint8_t *)prefix, 256);
}
