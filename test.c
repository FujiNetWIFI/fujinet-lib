#include "fujinet-fuji.h"
#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

AdapterConfigExtended ace;
char buffer[256];

extern void hexdump(uint8_t *buffer, int count);

int main()
{
#ifdef _CMOC_VERSION_
  *((uint8_t *) 0x95ac) = 57;
  *((uint8_t *) 0xff22) = 0x10;
#endif /* _CMOC_VERSION_ */

  printf("Searching for FujiNet...\n");
  if (!fuji_get_adapter_config_extended(&ace))
    strcpy(ace.fn_version, "FAIL");

  printf("FujiNet: %-14s  Make: ???\n", ace.fn_version);

  if (!fuji_set_host_prefix(1, "/"))
    printf("FAIL: fuji_set_host_prefix\n");

  if (!fuji_get_host_prefix(1, buffer))
    printf("FAIL: fuji_get_host_prefix\n");
  printf("Prefix: %s\n", buffer);

  return 0;
}
