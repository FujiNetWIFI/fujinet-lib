#include "fb_header.h"
#include "fujinet-fuji-coco.h"
#include "fujinet-network-coco.h"
#include "dw.h"

#include <fujinet-int.h>
#include <fujinet-bus.h>

fujibus_header fb_header;

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields, ...)
{
  uint8_t header_len;
  uint8_t aux[4];
  va_list ap;


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

  va_start(ap, fields);

  {
    uint8_t numbytes = fuji_field_numbytes(fields);

    if (numbytes > 0) aux[0] = va_arg(ap, uint8_t);
    if (numbytes > 1) aux[1] = va_arg(ap, uint8_t);
    if (numbytes > 2) aux[2] = va_arg(ap, uint8_t);
    if (numbytes > 3) aux[3] = va_arg(ap, uint8_t);
    if (numbytes)
      dwwrite(aux, numbytes);
  }

  if (fields & FUJI_FIELD_DATA) {
    const uint8_t *data = va_arg(ap, uint8_t *);
    const uint16_t data_length = va_arg(ap, uint16_t);

    if (data && data_length)
      dwwrite((uint8_t *) data, data_length);
  }

  {
    uint8_t *reply = NULL;
    uint16_t reply_length = 0;


    if (fields & FUJI_FIELD_REPLY) {
      reply = va_arg(ap, uint8_t *);
      reply_length = va_arg(ap, uint16_t);
    }

    va_end(ap);

    if (device == FUJI_DEVICEID_CLOCK) {
      if (reply)
        return dwread((uint8_t *) reply, reply_length);
      return true;
    }

    if (header_len == 3) { // FUJI_DEVICEID_NETWORK
      if (network_get_error(fb_header.fn.net.unit))
        return false;
      if (reply
          && network_get_response(fb_header.fn.net.unit, (uint8_t *) reply, reply_length))
        return false;
      return true;
    }

    if (fuji_get_error())
      return false;
    if (reply)
      return fuji_get_response((uint8_t *) reply, reply_length);
  }

  return true;
}
