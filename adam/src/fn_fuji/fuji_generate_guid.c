#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"
#include "response.h"

bool fuji_generate_guid(char *buffer)
{
  uint8_t err = 0;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,"\xBB",1);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  while(1)
    {
      err = eos_read_character_device(FUJINET_DEVICE_ID,buffer,MAX_GUID_LEN);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
