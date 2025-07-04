#include "fujinet-fuji.h"
#include "fujinet-cbm.h"
#include <string.h>

typedef struct {
  uint8_t opcode;
  uint8_t cmd;
  uint8_t data[258]; // FIXME - aux1 + aux2 + 256 byte path
} fujibus_packet;

fujibus_packet fb_packet;

bool fuji_bus_call(uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint16_t idx = 0, wlen, rlen;
  bool success = true;


  // These arguments are for compatibility with larger systems and not
  // used on Apple II. Hack to suppress the warning that they are
  // unused.
  (void) aux3;
  (void) aux4;

  fb_packet.opcode = 0x01;
  fb_packet.cmd = fuji_cmd;

  if (fields & FUJI_FIELD_AUX1)
    fb_packet.data[idx++] = aux1;
  if (fields & FUJI_FIELD_AUX2)
    fb_packet.data[idx++] = aux2;
  if (fields & FUJI_FIELD_DATA) {
    memcpy(&fb_packet.data[idx], data, data_length);
    idx += data_length;
  }

  if (fuji_cbm_open(CBM_CMD_CHANNEL, CBM_DEV_FUJI, CBM_CMD_CHANNEL, 2,
		    (unsigned char *) &fb_packet) != 0)
    return false;

  if (idx) {
    wlen = cbm_write(CBM_CMD_CHANNEL, fb_packet.data, idx);
    if (wlen != idx)
      success = false;
  }

  if (success && reply) {
    rlen = cbm_read(CBM_CMD_CHANNEL, reply, reply_length);
    if (rlen != reply_length)
      success = false;
  }

  // FIXME - does it matter if we don't bother to close it?
  cbm_close(CBM_CMD_CHANNEL);

  return success;
}
