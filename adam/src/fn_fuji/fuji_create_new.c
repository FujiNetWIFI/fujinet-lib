#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

bool fuji_create_new(NewDisk *new_disk)
{
  char nd[263]={0xE7,0x00,0x00,0x00,0x00,0x00,0x00};
  char *c = &nd[3];
  unsigned long *l = (unsigned long *)c;
  uint8_t err = 0;

  // fill in newdisk structure

  nd[1]=new_disk->hostSlot;
  nd[2]=new_disk->deviceSlot;
  *l=new_disk->numBlocks;
  strcpy(&nd[7],path);

  // Send command

  while(1)
    {
      err = eos_write_character_device(FUJINET_DEVICE_ID,&nd,sizeof(nd));

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
