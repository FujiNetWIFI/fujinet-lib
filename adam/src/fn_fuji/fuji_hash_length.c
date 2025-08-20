#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

extern unsigned char response[1024];

bool fuji_hash_length(uint8_t mode)
{
  uint8_t err = 0;

  // Send command

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,"\xC6",1);

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
      err = eos_read_character_device(FUJINET_DEVICE_ID,response,1024);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return (bool)response[0];
}
