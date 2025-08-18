#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_mount_all()
{
  uint8_t err = 0;

  while (1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,"\xD7",1);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        return FN_ERR_OK;
      else
        return FN_ERR_IO_ERROR;
    }
}
