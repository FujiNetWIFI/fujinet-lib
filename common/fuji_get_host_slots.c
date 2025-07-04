#include "fujinet-fuji.h"

#include <stdio.h>
#include <stdlib.h>
#define HEX_COLUMNS 8

void hexdump(uint8_t *buffer, int count)
{
  int outer, inner;
  uint8_t c;


  for (outer = 0; outer < count; outer += HEX_COLUMNS) {
    for (inner = 0; inner < HEX_COLUMNS; inner++) {
      if (inner + outer < count) {
	c = buffer[inner + outer];
	printf("%02x", c);
      }
      else
	printf("   ");
    }
    printf(" |");
    for (inner = 0; inner < HEX_COLUMNS && inner + outer < count; inner++) {
      c = buffer[inner + outer];
      if (c >= ' ' && c <= 0x7f)
	printf("%c", c);
      else
	printf(".");
    }
    printf("|\n");
  }

  return;
}

bool fuji_get_host_slots(HostSlot *h, size_t count)
{
  if (!FUJICALL(FUJICMD_READ_HOST_SLOTS))
    return false;
  if (!fuji_bus_get(h, sizeof(HostSlot) * count))
    return false;
#if 0
  hexdump((uint8_t *) h, sizeof(HostSlot) * count);
  exit(1);
#endif
  return true;
}
