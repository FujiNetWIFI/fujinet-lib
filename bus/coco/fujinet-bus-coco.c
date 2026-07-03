#include "fujinet-fuji-coco.h"
#include "fujinet-network-coco.h"
#include "dw.h"

#include <fujinet-int.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>
#include <fujinet-const.h>

typedef struct {
  uint16_t length;
  uint8_t data[MAX_APPKEY_LEN];
} FNAppKeyString;

typedef struct {
  uint8_t opcode;
  union {
    uint8_t cmd;
    struct {
      uint8_t unit;
      uint8_t cmd;
    } net;
  } fn;
} fujibus_header;

static fujibus_header fb_header;
FNAppKeyString appkey_buf;

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint8_t numbytes, idx;
  uint8_t header_len;
  bool success;
  uint8_t aux[4];


  if (device >= FUJI_DEVICEID_NETWORK
      && device <= FUJI_DEVICEID_NETWORK_LAST) {
    fb_header.opcode = OP_NET;
    fb_header.fn.net.unit = device - FUJI_DEVICEID_NETWORK + 1;
    fb_header.fn.net.cmd = fuji_cmd;
    header_len = 3;
  }
  else {
    if (device == FUJI_DEVICEID_CLOCK)
      fb_header.opcode = OP_CLOCK;
    else if (device == FUJI_DEVICEID_FUJINET)
      fb_header.opcode = OP_FUJI;
    else
      return false;

    fb_header.fn.cmd = fuji_cmd;
    header_len = 2;
  }

  bus_ready();
  dwwrite((unsigned char *) &fb_header, header_len);
  numbytes = fuji_field_numbytes(fields);
  idx = 0;
  if (numbytes) { aux[idx++] = aux1; numbytes--; }
  if (numbytes) { aux[idx++] = aux2; numbytes--; }
  if (numbytes) { aux[idx++] = aux3; numbytes--; }
  if (numbytes) { aux[idx++] = aux4; numbytes--; }
  if (idx)
    dwwrite(aux, idx);
  if (data && data_length)
    dwwrite((uint8_t *) data, data_length);

  if (device == FUJI_DEVICEID_CLOCK) {
    if (reply)
      return dwread((uint8_t *) reply, reply_length);
    return true;
  }

  if (header_len == 3) { // FUJI_DEVICEID_NETWORK
    success = !network_get_error(fb_header.fn.net.unit);
    if (success && reply)
      success = !network_get_response(fb_header.fn.net.unit,
                                      (uint8_t *) reply,
                                      reply_length);
  }
  else {
    success = !fuji_get_error();
    if (success && reply)
      success = fuji_get_response((uint8_t *) reply,
                                  reply_length);
  }

  return success;
}

uint16_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  fb_header.opcode = OP_NET;
  fb_header.fn.net.unit = device - FUJI_DEVICEID_NETWORK + 1;
  fb_header.fn.net.cmd = FUJICMD_READ;

  bus_ready();
  dwwrite((uint8_t *) &fb_header, sizeof(fb_header));
  dwwrite((uint8_t *) &length, sizeof(length));
  network_get_response(fb_header.fn.net.unit, (uint8_t *) buffer, length);

  return length;
}

uint16_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  fb_header.opcode = OP_NET;
  fb_header.fn.net.unit = device - FUJI_DEVICEID_NETWORK + 1;
  fb_header.fn.net.cmd = FUJICMD_WRITE;

  bus_ready();
  dwwrite((uint8_t *) &fb_header, sizeof(fb_header));
  dwwrite((uint8_t *) &length, sizeof(length));
  dwwrite((uint8_t *) buffer, length);

  if (network_get_error(fb_header.fn.net.unit))
    length = 0;
  return length;
}
