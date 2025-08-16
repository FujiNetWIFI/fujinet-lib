#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

// Borrow the response buffer.
extern unsigned char response[1024];

bool fuji_base64_decode_input(char *s, uint16_t len)
{
  uint8_t err = 0;
  uint16_t o = 0; // offset
  
  while(len)
    {
      uint16_t l = (len > 1024 ? 1024 : len);

      // Fill command + payload
      response[0] = 0xCC; // Command
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
