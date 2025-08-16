#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

extern uint8_t response[1024];

bool fuji_get_device_filename(uint8_t ds, char *buffer)
{
  uint8_t err = 0;
  uint8_t gds[2] = {0xDA,0x00};

  gds[1] = ds;

  // send command
  
  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&gds,sizeof(gds));

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }

  // get response

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

  if (buffer)
    strcpy(buffer,(const char *)&response[0]);

  return FN_ERR_OK;
}
