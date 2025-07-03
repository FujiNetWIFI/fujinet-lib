#include "fujinet-bus-apple2.h"
#include <string.h>

bool fuji_bus_call(uint8_t fuji_cmd)
{
  if (sp_get_fuji_id() == 0) {
    return false;
  }

  sp_error = sp_status(sp_fuji_id, fuji_cmd);
  return !sp_error;
}

bool fuji_bus_get(void *data, size_t length)
{
  memcpy(data, &sp_payload[0], length);
  //hexdump(data, length);
  return true;
}

bool fuji_bus_put(void *data, size_t length)
{
  memcpy(sp_payload, data, length);
  return true;
}
