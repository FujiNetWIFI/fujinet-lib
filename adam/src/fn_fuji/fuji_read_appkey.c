#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-network-adam.h"
#include <eos.h>
#include "response.h"

uint16_t fn_adam_creator_id;
uint8_t fn_adam_app_id;
enum AppKeySize fn_adam_keysize;

bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
{
  unsigned char err=0;
  DCB *dcb = 0;

  struct
  {
    unsigned char cmd;
    unsigned short creator;
    unsigned char app;
    unsigned char key;
    char data[64];
  } a;
  
  a.cmd = FUJICMD_READ_APPKEY;
  a.creator = fn_adam_creator_id;
  a.app = fn_adam_app_id;
  a.key = key_id;

  while(1)
  {
    err = eos_write_character_device(FUJINET_DEVICE_ID,(unsigned char *)&a,sizeof(a));

    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
      break;
    else
      return false;
  }

  //get the DCB...
    dcb = eos_find_dcb(FUJINET_DEVICE_ID); // Replace with net device


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

  memcpy (data, response,MAX_APPKEY_LEN);
  *count = dcb->len;
  return true;
}

