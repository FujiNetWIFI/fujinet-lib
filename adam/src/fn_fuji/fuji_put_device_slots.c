#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_put_device_slots(DeviceSlot *d, size_t size)
{
  unsigned char pds[305] = {0xF1};
  uint8_t err = 0;
  
  memcpy(&pds[1],d,304);

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&pds,sizeof(pds));

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }
  
  return FN_ERR_OK;
}
