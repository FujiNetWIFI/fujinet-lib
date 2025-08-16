#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_disable_device(uint8_t d)
{
  uint8_t err=0;

  char dd[2] = {0xD4,0x00};

  dd[1] = d;
  
  while (1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&dd,2);

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	return FN_ERR_OK;
      else
	return FN_ERR_IO_ERROR;
    }
}
