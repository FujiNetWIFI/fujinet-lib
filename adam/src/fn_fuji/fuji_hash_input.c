#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"
#include "response.h"

bool fuji_hash_input(char *s, uint16_t len)
{
  uint8_t err = 0;
  uint16_t o = 0; // offset

  while(len)
    {
      uint16_t l = (len > RESPONSE_SIZE ? RESPONSE_SIZE : len);

      // Fill command + payload
      response[0] = 0xC8; // Command
      memcpy(&response[1],&s[o],l);

      err = eos_write_character_device(FUJINET_DEVICE_ID,response,l);

      if (err = ADAMNET_TIMEOUT)
        continue; // Retry
      else if (err == ADAMNET_OK)
        {
          len -= l;
          o += l;
          continue; // Do the next chunk.
        }
      else
        return FN_ERR_IO_ERROR; // We failed.
    }
}
