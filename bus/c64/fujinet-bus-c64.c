#include "fujinet-fuji.h"
#include "fujinet-cbm.h"
#include <stdarg.h>

#include <stdio.h> // debug

typedef struct {
  uint8_t opcode;
  uint8_t cmd;
  uint8_t aux[4];
} fujibus_packet;

static fujibus_packet fb_packet;

//uint8_t fn_device_error;

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields, ...)
{
  uint16_t rlen;
  uint16_t idx, numbytes;
  uint8_t cbm_chan, cbm_dev;
  const uint8_t *data = NULL;
  uint16_t data_length = 0;
  va_list ap;
  bool success = true;


  fb_packet.opcode = OPCODE_NO_PAYLOAD;
  fb_packet.cmd = fuji_cmd;

  va_start(ap, fields);

  idx = 0;
  numbytes = fuji_field_numbytes(fields);
  if (numbytes > 0)
    fb_packet.aux[idx++] = va_arg(ap, uint8_t);
  if (numbytes > 1)
    fb_packet.aux[idx++] = va_arg(ap, uint8_t);
  if (numbytes > 2)
    fb_packet.aux[idx++] = va_arg(ap, uint8_t);
  if (numbytes > 3)
    fb_packet.aux[idx++] = va_arg(ap, uint8_t);
  if (fields & FUJI_FIELD_DATA) {
    data = va_arg(ap, uint8_t *);
    data_length = va_arg(ap, uint16_t);
  }

  if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST) {
    cbm_chan = CBM_CMD_CHANNEL;
    //cbm_chan = device - FUJI_DEVICEID_NETWORK + CBM_DATA_CHANNEL_0;
    cbm_dev = CBM_DEV_NETWORK;
  }
  else {
    cbm_chan = CBM_CMD_CHANNEL;
    cbm_dev = CBM_DEV_FUJI;
  }

  if (idx || data_length)
    fb_packet.opcode = OPCODE_HAS_PAYLOAD;

  if (fuji_cbm_open(cbm_chan, cbm_dev, cbm_chan, 2, (unsigned char *) &fb_packet) != 0) {
    va_end(ap);
    return false;
  }

  // The firmware runs the command when the payload's LISTEN ends, so the
  // aux bytes and the data must go out in one output session.
  if (fb_packet.opcode == OPCODE_HAS_PAYLOAD) {
    if (cbm_k_ckout(cbm_chan))
      success = false;
    else {
      uint16_t pos;


      for (pos = 0; pos < idx; pos++)
        cbm_k_bsout(fb_packet.aux[pos]);
      for (pos = 0; pos < data_length; pos++)
        cbm_k_bsout(data[pos]);
      if (cbm_k_readst())
        success = false;
    }
    cbm_k_clrch();
  }

  if (success && (fields & FUJI_FIELD_REPLY)) {
    uint8_t *reply = va_arg(ap, uint8_t *);
    uint16_t reply_length = va_arg(ap, uint16_t);


    rlen = cbm_read(CBM_CMD_CHANNEL, reply, reply_length);
  }

  va_end(ap);

  // FIXME - does it matter if we don't bother to close it?
  cbm_close(CBM_CMD_CHANNEL);

  return success;
}

uint16_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  if (!NETCALL_B12_RV(NETCMD_READ, device - FUJI_DEVICEID_NETWORK + 1 + CBM_DATA_CHANNEL_0,
                      length, buffer, length))
    return 0;
  return length;
}

uint16_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  if (!NETCALL_D(NETCMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1 + CBM_DATA_CHANNEL_0,
                 buffer, length))
    return 0;
  return length;
}

/*
  appkeys are variable length strings. Because IEC supports
  variable length data packets, aux1/aux2 aren't used to send the
  length of the string. Instead only the string data is sent with no
  length field, no block size, no padding.
*/

bool fuji_bus_appkey_read(void *, uint16_t *)
{
  return false;
}

bool fuji_bus_appkey_write(void *string, uint16_t length)
{
  return FUJICALL_D(FUJICMD_WRITE_APPKEY, string, length);
}
