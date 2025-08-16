#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_set_directory_position(uint16_t pos)
{
  uint8_t sdp[3]={0xE4,0x00,0x00};
  uint8_t err = 0;

  memcpy(&sdp[1],&pos,sizeof(uint16_t));
  
  while (1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&sdp,sizeof(sdp));

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }
  
  return FN_ERR_OK;
}
