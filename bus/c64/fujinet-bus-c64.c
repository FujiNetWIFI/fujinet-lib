#include "fujinet-fuji.h"
#include "fujinet-cbm.h"
#include <string.h>

#include <stdio.h> // debug

typedef struct {
  uint8_t opcode;
  uint8_t cmd;
  uint8_t data[258]; // FIXME - aux1 + aux2 + 256 byte path
} fujibus_packet;

static fujibus_packet fb_packet;

uint8_t fn_device_error;

bool fuji_net_call(uint8_t device, uint8_t unit, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
}

bool fuji_bus_call(uint8_t device, uint8_t unit, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint16_t idx = 0, wlen, rlen;
  bool success = true;


  fb_packet.opcode = 0x01;
  fb_packet.cmd = fuji_cmd;

  if (fields & FUJI_FIELD_AUX1)
    fb_packet.data[idx++] = aux1;
  if (fields & FUJI_FIELD_AUX2)
    fb_packet.data[idx++] = aux2;
  if (fields & FUJI_FIELD_AUX3)
    fb_packet.data[idx++] = aux3;
  if (fields & FUJI_FIELD_AUX4)
    fb_packet.data[idx++] = aux4;
  if (data) {
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

uint16_t fuji_bus_read(uint8_t device, uint8_t unit, void *buffer, size_t length)
{
  uint16_t rlen;
  printf("FUJI_BUS_READ %u\n", length);
  rlen = cbm_read(unit + CBM_DATA_CHANNEL_0, buffer, length);
  printf("FBR GOT %u\n", rlen);
  return rlen;
}

uint16_t fuji_bus_write(uint8_t device, uint8_t unit, const void *buffer, size_t length)
{
  return cbm_write(unit + CBM_DATA_CHANNEL_0, buffer, length);
}

/*
  appkeys are variable length strings. Because IEC supports
  variable length data packets, aux1/aux2 aren't used to send the
  length of the string. Instead only the string data is sent with no
  length field, no block size, no padding.
*/

bool fuji_bus_appkey_read(void *string, uint16_t *length)
{
  uint16_t rlen;


  rlen = cbm_read(CBM_CMD_CHANNEL, string, MAX_APPKEY_LEN);
  *length = rlen;
  return true;
}

bool fuji_bus_appkey_write(void *string, uint16_t length)
{
  return FUJICALL_D(FUJICMD_WRITE_APPKEY, string, length);
}
