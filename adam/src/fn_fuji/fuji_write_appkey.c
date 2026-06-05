#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-network-adam.h"
#include <eos.h>

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
  unsigned char err=0;
  unsigned char sendbuf[MAX_APPKEY_LEN + 1];

  struct
  {
    unsigned char cmd;
    unsigned short creator;
    unsigned char app;
    unsigned char key;
    unsigned char write;
    unsigned char reserved;
  } open_key;

  if (count > MAX_APPKEY_LEN)
    return false;
    
  open_key.cmd = FUJICMD_OPEN_APPKEY;
  open_key.creator = fn_adam_creator_id;
  open_key.app = fn_adam_app_id;
  open_key.key = key_id;
  open_key.write = 1; // Open for Writing
  open_key.reserved = 0; // Reserved

  while(1)
  {
    err = eos_write_character_device(FUJINET_DEVICE_ID,(unsigned char *)&open_key,sizeof(open_key));

    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
      break;
    else
      return false;
  }

  sendbuf[0] = FUJICMD_WRITE_APPKEY;
  strncpy(&sendbuf[1],data,count);

  while(1)
  {
    err = eos_write_character_device(FUJINET_DEVICE_ID,sendbuf, count+1);

    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
      break;
    else
      return false;
  }

  return true;
}

