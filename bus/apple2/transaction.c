#include "fujinet-bus-apple2.h"
#include <string.h>

#include <stdio.h>

void hexdump(uint8_t *buffer, int count)
{
  int outer, inner;
  uint8_t c;


  for (outer = 0; outer < count; outer += 16) {
    for (inner = 0; inner < 16; inner++) {
      if (inner + outer < count) {
	c = buffer[inner + outer];
	printf("%02x ", c);
      }
      else
	printf("   ");
    }
    printf(" |");
    for (inner = 0; inner < 16 && inner + outer < count; inner++) {
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

bool transaction_call(uint8_t fuji_cmd)
{
  if (sp_get_fuji_id() == 0) {
    return false;
  }

  sp_error = sp_status(sp_fuji_id, fuji_cmd);
  printf("DID CALL %i %i: %i\n", sp_fuji_id, fuji_cmd, sp_error);
  return !sp_error;
}

bool transaction_get(void *data, size_t length)
{
  memcpy(data, &sp_payload[0], length);
  //hexdump(data, length);
  return true;
}

bool transaction_put(void *data, size_t length)
{
  memcpy(sp_payload, data, length);
  return true;
}
