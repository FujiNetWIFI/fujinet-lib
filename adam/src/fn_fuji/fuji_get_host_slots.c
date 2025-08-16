#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

bool fuji_get_host_slots(HostSlot *h, size_t size)
{
  uint8_t err = 0;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,"\xF4",1);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return false;
    }

  while(1)
    {
      err = eos_read_character_device(FUJINET_DEVICE_ID,response,RESPONSE_SIZE);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return false;
    }

  if (h)
    memcpy(h, response, 256); // yes, I know, hard coded. bad me.
  return true;
}
