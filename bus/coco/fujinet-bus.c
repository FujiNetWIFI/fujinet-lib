#include "fujinet-bus.h"
#include "fujinet-fuji-coco.h"
#include "dw.h"

bool fuji_bus_call(uint8_t fuji_cmd)
{
  struct _gace
  {
    uint8_t opcode;
    uint8_t cmd;
  } gace;

  gace.opcode = OP_FUJI;
  gace.cmd = fuji_cmd;

  bus_ready();

  dwwrite((uint8_t *)&gace, sizeof(gace));
  if (fuji_get_error())
    return false;

  return true;
}

bool fuji_bus_get(void *data, size_t length)
{
  return (bool) fuji_get_response((unsigned char *) data, length);
}

bool fuji_bus_put(void *data, size_t length)
{
  return false;
}
