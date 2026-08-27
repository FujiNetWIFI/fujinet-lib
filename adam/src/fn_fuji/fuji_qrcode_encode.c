#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten)
{
  uint8_t err = 0;
  uint8_t ch[4] = {0xBD,0x00,0x00,0x00};

  ch[1] = version;
  ch[2] = ecc;
  ch[3] = shorten;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&ch,sizeof(ch));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
