#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_hash_compute(uint8_t type)
{
  uint8_t err = 0;
  uint8_t ch[2] = {0xC7,0x00};

  ch[1] = type;
  
  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&ch,sizeof(ch));

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }
  
  return FN_ERR_OK;
}
