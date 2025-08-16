#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
{
  char cf[259] = {0xD8,0x00,0x00};
  DCB *dcb = NULL;
  uint8_t err = 0;

  cf[1]=src_slot;
  cf[2]=dst_slot;
  strcpy(&cf[3],copy_spec);

  // Send command

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,cf,sizeof(cf));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return FN_ERR_IO_ERROR;
    }

  // Poll status until complete

  while(1)
    {
      err = eos_request_device_status(FUJINET_DEVICE_ID,dcb);

      if (err == ADAMNET_TIMEOUT)
        continue;
    }

  return err;
}
