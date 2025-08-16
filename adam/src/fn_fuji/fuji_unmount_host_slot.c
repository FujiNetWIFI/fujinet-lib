#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_unmount_host_slot(uint8_t hs)
{
  uint8_t err = 0;
  uint8_t uhi[2] = {0xE6,0x00};

  uhi[1] = hs;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&uhi,sizeof(uhi));

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }
  
  return FN_ERR_OK;
}
