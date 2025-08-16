#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

extern unsigned char response[1024];

bool fuji_base64_decode_output(char *s, uint16_t len)
{
  uint8_t err = 0;
  uint16_t o = 0, lensave = len;
  
  // Send command
  
  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,"\xC9",1);

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }

  // Get response

  while(len)
    {
      uint16_t l = (len > 1024 ? 1024 : len);
      
      err = eos_read_character_device(FUJINET_DEVICE_ID,response,1024);

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	{
	  memcpy(&s[o],&response[o],l);
	  len -= l;
	  o += l;
	}
      else
	return FN_ERR_IO_ERROR;
    }

  return FN_ERR_OK;
}
