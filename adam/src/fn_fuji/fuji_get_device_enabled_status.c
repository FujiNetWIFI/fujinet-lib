#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"
#include "response.h"

bool fuji_get_device_enabled_status(uint8_t d)
{
  uint8_t err = 0;
  uint8_t des[2] = {0xD1,0x00};

  des[1] = d;

  // Send command

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&des,sizeof(des));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  // Get response

  while(1)
    {
      err = eos_read_character_device(FUJINET_DEVICE_ID,response,RESPONSE_SIZE);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return (bool)response[0];
}
