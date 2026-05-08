#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"
#include "response.h"

bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
  uint8_t rd[3] = {0xF6,0x00,0x00};
  uint8_t err = 0;

  rd[1] = maxlen;
  rd[2] = aux2;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&rd,sizeof(rd));

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

  if (buffer)
    strcpy(buffer,(const char *)response);

  return FN_ERR_OK;
}
