#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_set_boot_mode(uint8_t mode)
{
  uint8_t sbm[2] = {0xD6,0x00};
  uint8_t err = 0;

  sbm[1] = mode;
  
  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&sbm,sizeof(sbm));

      if (err == ADAMNET_TIMEOUT)
	continue;
      else if (err == ADAMNET_OK)
	break;
      else
	return FN_ERR_IO_ERROR;
    }
  
  return FN_ERR_OK;
}
