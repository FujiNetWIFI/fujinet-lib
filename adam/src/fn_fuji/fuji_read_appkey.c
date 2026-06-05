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
    unsigned char write;
    unsigned char reserved;
  } open_key;

  open_key.cmd = FUJICMD_OPEN_APPKEY;
  open_key.creator = fn_adam_creator_id;
  open_key.app = fn_adam_app_id;
  open_key.key = key_id;
  open_key.write = 0; // Open for Reading
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

  open_key.cmd = FUJICMD_READ_APPKEY;

  while(1)
  {
    err = eos_write_character_device(FUJINET_DEVICE_ID,(unsigned char *)&open_key,1);

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

  *count = dcb->len;

  if (*count <= MAX_APPKEY_LEN) {
    memcpy (data, response,*count);
    return true;
  } else
    return false;
}

