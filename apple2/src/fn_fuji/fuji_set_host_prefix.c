#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_set_host_prefix(uint8_t hs, char *prefix)
{
  // Not implemented in A2
  size_t filename_len = strlen(prefix);
  if (filename_len >= 254)
    {
      return false;
    }

  sp_error = sp_get_fuji_id();
  if (sp_error <= 0)
    {
      return false;
    }

  sp_payload[0] = (filename_len + 1) & 0xFF;
  sp_payload[1] = 0;
  sp_payload[2] = hs;
  strcpy((char *) &sp_payload[3], prefix);

  sp_error = sp_control(sp_fuji_id, FUJICMD_SET_HOST_PREFIX);
  
  return sp_error == 0;
}
