#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

bool fuji_unmount_disk_image(uint8_t ds)
{
  uint8_t err = 0;
  uint8_t udi[2] = {0xE9,0x00};

  udi[1] = ds;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&udi,sizeof(udi));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
