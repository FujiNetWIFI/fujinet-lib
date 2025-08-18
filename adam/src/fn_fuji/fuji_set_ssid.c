#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_set_ssid(NetConfig *nc)
{
  uint8_t ss[98] = {0xFB};
  uint8_t err = 0;

  memcpy(&ss[98],nc,sizeof(NetConfig));

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&ss,sizeof(ss));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
