#include "fujinet-bus-msx.h"
#include "fujinet-commands.h"
#include "fujinet-unapi-msx.h"

#undef HEXDUMP
#if defined(DEBUG) || defined(HEXDUMP)
#define COLUMNS 16

static void hexdump(uint8_t *buffer, int count)
{
  int outer, inner;
  uint8_t c;


  for (outer = 0; outer < count; outer += COLUMNS) {
    for (inner = 0; inner < COLUMNS; inner++) {
      if (inner + outer < count) {
	c = buffer[inner + outer];
	printf("%02x ", c);
      }
      else
	printf("   ");
    }
    printf(" |");
    for (inner = 0; inner < COLUMNS && inner + outer < count; inner++) {
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
#endif /* HEXDUMP */

/* Static only to avoid building it on the stack twice: fuji_unapi_call() moves
   it somewhere the implementation can see before entering one.  Where BSS
   lands is not safe by itself - page 3 in a ROM, but page 1 in an MSX-DOS .COM
   of any size. */
static FujiNetParams params;

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
                   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
                   const void *buf, size_t buf_length)
{
  params.device = device;
  params.command = fuji_cmd;
  params.aux_descr = fields;

  params.aux1 = aux1;
  params.aux2 = aux2;
  params.aux3 = aux3;
  params.aux4 = aux4;

  params.buffer = (void *) buf;
  params.length = buf_length;

  if (fields & FUJI_FIELD_REPLY) {
#ifdef DEBUG
    printf("FUJINET READ %d\n", params.length);
    hexdump(&params, sizeof(params));
#endif
    return fuji_unapi_call(FUJI_CALL_READ, &params);
  }

#ifdef DEBUG
  printf("FUJINET WRITE %d\n", params.length);
#endif
  return fuji_unapi_call(FUJI_CALL_WRITE, &params);
}

uint16_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  NETCALL_B12_RV(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}

uint16_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_B12_D(FUJICMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}
