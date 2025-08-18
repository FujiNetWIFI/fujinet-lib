#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

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
	return FN_ERR_IO_ERROR;
    }

  while(1)
    {
      err = eos_read_character_device(FUJINET_DEVICE_ID,response,1024);

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }

  if (d)
    memcpy(d, response, 256); // yes, I know, hard coded. bad me.
}
