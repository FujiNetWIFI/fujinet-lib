#include "fujinet-fuji.h"
#include "fujinet-cbm.h"
#include <string.h>

typedef struct {
  uint8_t opcode;
  uint8_t cmd;
  uint8_t data[256];
} fujibus_packet;

fujibus_packet fb_packet;
static bool fuji_is_open = false;

bool fuji_bus_call(uint8_t fuji_cmd, uint8_t fields,
                   uint8_t aux1, uint8_t aux2,
                   const void *data, size_t length)
{
  uint16_t idx = 0, wlen;


  fb_packet.opcode = 0x01;
  fb_packet.cmd = fuji_cmd;

  if (fields & FUJI_FIELD_AUX1)
    fb_packet.data[idx++] = aux1;
  if (fields & FUJI_FIELD_AUX2)
    fb_packet.data[idx++] = aux2;
  if (fields & FUJI_FIELD_DATA) {
    memcpy(&fb_packet.data[idx], data, length);
    idx += length;
  }

  if (fuji_is_open) {
    cbm_close(CBM_CMD_CHANNEL);
    fuji_is_open = false;
  }

  if (fuji_cbm_open(CBM_CMD_CHANNEL, CBM_DEV_FUJI, CBM_CMD_CHANNEL, 2,
		    (unsigned char *) &fb_packet) != 0)
    return false;

  fuji_is_open = true;

  if (length) {
    wlen = cbm_write(CBM_CMD_CHANNEL, fb_packet.data, length);
    if (wlen != length)
      return false;
  }

  // FIXME - does it matter if we don't bother to close it right away?

  return true;
}

bool fuji_bus_get(void *data, size_t length)
{
  uint16_t rlen;


  rlen = cbm_read(CBM_CMD_CHANNEL, data, length);
  cbm_close(CBM_CMD_CHANNEL);
  fuji_is_open = false;
  if (rlen != length)
    return false;
  return true;
}
