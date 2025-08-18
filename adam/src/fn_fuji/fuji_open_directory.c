#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_open_directory(uint8_t hs, char *path_filter)
{
  uint8_t err = 0;
  uint8_t od[258] = {0xF7,0x00};
  char *e;

  memset(&od,0,258);
  od[0]=0xF7;
  od[1]=hs;
  strcpy((char *)&od[2],path_filter);

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&od,sizeof(od));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
