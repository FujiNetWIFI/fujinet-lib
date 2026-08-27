#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"
#include "response.h"

bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len)
{
  uint8_t err = 0;
  uint8_t ch[2] = {0xBE,0x00};

  ch[1] = output_mode;

  // Send command

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

  if (len)
    *len = (unsigned long)response[0]
         | ((unsigned long)response[1] << 8)
         | ((unsigned long)response[2] << 16)
         | ((unsigned long)response[3] << 24);

  return FN_ERR_OK;
}
