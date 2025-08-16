#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_put_host_slots(HostSlot *h, size_t size)
{
  unsigned char phs[257] = {0xF3};
  uint8_t err = 0;
  
  memcpy(&phs[1],d,256);

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&phs,sizeof(phs));

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }
  
  return FN_ERR_OK;
}
