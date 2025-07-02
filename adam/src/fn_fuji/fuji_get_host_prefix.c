#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include <eos.h>
#include <string.h>

extern unsigned char response[1024];

bool fuji_get_host_prefix(uint8_t hs, char *prefix)
{
  struct _ghp
  {
    unsigned char cmd;
    unsigned char hs;
  } ghp;

  memset(response,0,sizeof(response));
  ghp.cmd = 0xE0;
  ghp.hs = hs;

  if (eos_write_character_device(0x0f,ghp,sizeof(ghp)) != 0x80)
    return false;

  if (eos_read_character_device(0x0f,response,sizeof(response)) != 0x80)
    return false;

  strcpy(prefix,response);
  
  return true;
}
