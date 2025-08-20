#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

extern uint8_t response[1024];

bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
  uint8_t err = 0;
  uint8_t gsr[2] = {0xFC,0x00};

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&gsr,sizeof(gsr));

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

  if (ssid_info)
    memcpy(ssid_info,response,sizeof(SSIDInfo));

  return FN_ERR_OK;
}
