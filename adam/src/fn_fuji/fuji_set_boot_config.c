#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

bool fuji_set_boot_config(uint8_t toggle)
{
  uint8_t sbc[2] = {0xD9,0x00};
  uint8_t err = 0;

  sbc[1] = toggle;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&sbc,sizeof(sbc));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
