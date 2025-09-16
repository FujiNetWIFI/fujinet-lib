#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
{
  // hs is not used.
  // neither is mode
  uint8_t err = 0;
  char sdf[258] = {0xE2,0x00};

  sdf[1] = ds;

  strcpy(&sdf[2],buffer);

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&sdf,sizeof(sdf));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
