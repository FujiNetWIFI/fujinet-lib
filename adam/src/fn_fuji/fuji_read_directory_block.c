#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"
#include "response.h"

bool fuji_read_directory_block(uint8_t ram_pages, uint8_t group_size, void *buffer)
{
  uint8_t rd[3] = {0xF6,0x00,0x00};
  uint8_t err = 0;

  // AdamNet caps a character message at RESPONSE_SIZE (the max message size the
  // Fuji device reports in its status), and the reply is ram_pages * 256 bytes.
  if (ram_pages == 0 || (uint16_t)ram_pages * 256 > RESPONSE_SIZE)
    return false;

  if (group_size == 0 || group_size > 0x3F)
    return false;

  rd[1] = ram_pages;
  rd[2] = 0xC0 | group_size;

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&rd,sizeof(rd));

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return false;
    }

  while(1)
    {
      err = eos_read_character_device(FUJINET_DEVICE_ID,response,RESPONSE_SIZE);

      if (err == ADAMNET_TIMEOUT)
        continue;
      else if (err == ADAMNET_OK)
        break;
      else
        return false;
    }

  if (buffer)
    memcpy(buffer, response, (uint16_t)ram_pages * 256);

  return true;
}
