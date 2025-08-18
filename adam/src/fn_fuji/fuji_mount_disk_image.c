#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
{
  uint8_t err = 0;
  uint8_t mdi[3] = {0xF8,0x00,0x00};

  mdi[1] = ds;
  mdi[2] = mode;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&mdi,sizeof(mdi));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
