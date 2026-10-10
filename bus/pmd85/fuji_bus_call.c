#include <fujinet-int.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>

#include "fujinet-fuji-pmd85.h"
#include "fujinet-network-pmd85.h"
#include "dw.h"

// opcode, unit (network only), command, then up to 4 aux bytes
static uint8_t fb_packet[7];

// The firmware puts some 16 and 32 bit values at the start of a data
// or reply block in bus order, which on DriveWire is high byte first.
// Returns the size of that value, 0 if the command has none.
static uint8_t payload_int_size(uint8_t device, uint8_t fuji_cmd)
{
  if (device == FUJI_DEVICEID_FUJINET) {
    switch (fuji_cmd) {
    case FUJICMD_OPEN_APPKEY: // creator id
    case FUJICMD_READ_APPKEY: // length of the key
      return 2;

    case FUJICMD_BASE64_ENCODE_LENGTH:
    case FUJICMD_BASE64_DECODE_LENGTH:
    case FUJICMD_QRCODE_LENGTH:
      return 4;
    }
  }
  else if (device != FUJI_DEVICEID_CLOCK && fuji_cmd == FUJICMD_STATUS)
    return 2; // bytes waiting

  return 0;
}

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
                   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
                   const void *buf, size_t buf_length)
{
  uint8_t *aux;
  const uint8_t *data;
  uint8_t header_len, wide, idx, tmp;
  uint8_t swapped[4];
  uint8_t unit = 0;
  uint8_t numbytes = fuji_field_numbytes(fields);
  uint8_t *reply = NULL;


  if (device >= FUJI_DEVICEID_NETWORK
      && device <= FUJI_DEVICEID_NETWORK_LAST) {
    unit = device - FUJI_DEVICEID_NETWORK + 1;
    fb_packet[0] = OP_NET;
    fb_packet[1] = unit;
    fb_packet[2] = fuji_cmd;
    header_len = 3;

    // DriveWire network commands *must* have aux1 and aux2, the
    // unused ones are already zero
    if (numbytes < 2)
      numbytes = 2;
  }
  else {
    if (device == FUJI_DEVICEID_CLOCK)
      fb_packet[0] = OP_CLOCK;
    else if (device == FUJI_DEVICEID_FUJINET)
      fb_packet[0] = OP_FUJI;
    else
      return false;

    fb_packet[1] = fuji_cmd;
    header_len = 2;
  }

  // DriveWire wants 16 and 32 bit values high byte first, but they
  // were split into the aux bytes low byte first.
  aux = &fb_packet[header_len];
  switch (fields & 0x07) {
  case FUJI_FIELD_B12:
  case FUJI_FIELD_B12_B34:
    aux[0] = aux2;
    aux[1] = aux1;
    aux[2] = aux4;
    aux[3] = aux3;
    break;

  case FUJI_FIELD_C1234:
    aux[0] = aux4;
    aux[1] = aux3;
    aux[2] = aux2;
    aux[3] = aux1;
    break;

  default:
    aux[0] = aux1;
    aux[1] = aux2;
    aux[2] = aux3;
    aux[3] = aux4;
    break;
  }

  bus_ready();
  dwwrite(fb_packet, header_len + numbytes);

  wide = payload_int_size(device, fuji_cmd);
  if (buf_length < wide)
    wide = 0;

  if ((fields & FUJI_FIELD_DATA) && buf && buf_length) {
    data = (const uint8_t *) buf;
    if (wide) {
      // Caller's copy stays in native order
      for (idx = 0; idx < wide; idx++)
        swapped[idx] = data[wide - 1 - idx];
      dwwrite(swapped, wide);
      data += wide;
      buf_length -= wide;
    }
    if (buf_length)
      dwwrite(data, buf_length);
  }

  if (fields & FUJI_FIELD_REPLY)
    reply = (uint8_t *) buf;

  if (device == FUJI_DEVICEID_CLOCK) {
    if (reply)
      return dwread(reply, buf_length);
    return true;
  }

  if (header_len == 3) { // FUJI_DEVICEID_NETWORK
    if (network_get_error(unit))
      return false;
    if (reply && network_get_response(unit, reply, buf_length))
      return false;
  }
  else {
    if (fuji_get_error())
      return false;
    if (reply && !fuji_get_response(reply, buf_length))
      return false;
  }

  if (reply && wide) {
    for (idx = 0; idx < wide / 2; idx++) {
      tmp = reply[idx];
      reply[idx] = reply[wide - 1 - idx];
      reply[wide - 1 - idx] = tmp;
    }
  }

  return true;
}
