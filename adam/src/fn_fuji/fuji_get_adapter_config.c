#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

extern unsigned char response[1024];

bool fuji_get_adapter_config(AdapterConfig *ac)
{
  uint8_t err = 0;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,"\xE8",1);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  while(1)
    {
      err = eos_read_character_device(FUJINET_DEVICE_ID,response,sizeof(response));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  if (ac)
    memcpy(ac,response,sizeof(AdapterConfig));

  return FN_ERR_OK;
}
