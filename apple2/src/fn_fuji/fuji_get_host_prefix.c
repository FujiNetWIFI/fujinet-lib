#include <stdint.h>
#include "fujinet-fuji.h"

// A8 to AF are now being allocated as get host prefix for slots 0 to 7.
bool fuji_get_host_prefix(uint8_t hs, char *prefix)
{
  uint8_t stat = ds + 0xA8;
  
  if (sp_get_fuji_id() == 0) {
    return false;
  }
  
  sp_error = sp_status(sp_fuji_id, stat);
  if (sp_error == 0) {
    memcpy(buffer, &sp_payload[0], 256);
  }
  return sp_error == 0;
}
