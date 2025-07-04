#include "fujinet-bus.h"
#include "fujinet-fuji-coco.h"
#include "dw.h"

typedef struct {
  uint8_t opcode;
  uint8_t cmd;
  uint8_t data[256];
} fujibus_packet;

fujibus_packet fb_packet;

bool fuji_bus_call(uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2,
		   const void *data, size_t length)
{
  uint16_t idx = 0;


  fb_packet.opcode = OP_FUJI;
  fb_packet.cmd = fuji_cmd;

  if (fields & FUJI_FIELD_AUX1)
    fb_packet.data[idx++] = aux1;
  if (fields & FUJI_FIELD_AUX2)
    fb_packet.data[idx++] = aux2;
  if (fields & FUJI_FIELD_DATA) {
    memcpy(&fb_packet.data[idx], data, length);
    idx += length;
  }

  bus_ready();
  dwwrite((unsigned char *) &fb_packet, 2 + idx);
  return !fuji_get_error();
}

bool fuji_bus_get(void *data, size_t length)
{
  return (bool) fuji_get_response((unsigned char *) data, length);
}
