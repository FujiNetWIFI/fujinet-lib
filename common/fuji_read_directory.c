#include "fujinet-fuji.h"

bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
  if (!FUJICALL_A1_A2(FUJICMD_READ_DIR_ENTRY, maxlen, aux2))
    return false;
  if (!fuji_bus_get(buffer, maxlen))
    return false;
  return true;
}
