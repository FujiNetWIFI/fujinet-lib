#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

bool fuji_mount_host_slot(uint8_t hs)
{
  uint8_t err = 0;
  uint8_t mhi[2] = {0xF9,0x00};

  mhi[1] = hs;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&mhi,sizeof(mhi));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}


