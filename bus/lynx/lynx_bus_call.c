#include "fujinet-fuji.h"
#include "fujinet-int.h"
#include "lynxfnio.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

char _lynx_packet[1024];
uint16_t _lynx_len;
uint16_t _comlynx_init = 0;

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *buf, size_t buf_length)
{
  uint8_t r;
  uint8_t numbytes;


  // Is Comlynx initialized?
  if (!_comlynx_init) {
    if (!fnio_init())
      return(false);
    _comlynx_init = 1;
  }

  // Reset our data length counter
  _lynx_len = 0;

  // Command is first
  _lynx_packet[_lynx_len] = fuji_cmd;
  _lynx_len++;

  // Build the packet from aux values
  numbytes = fuji_field_numbytes(fields);
  if (numbytes > 0)
    _lynx_packet[_lynx_len++] = aux1;
  if (numbytes > 1)
    _lynx_packet[_lynx_len++] = aux2;
  if (numbytes > 2)
    _lynx_packet[_lynx_len++] = aux3;
  if (numbytes > 3)
    _lynx_packet[_lynx_len++] = aux4;

  // Add data if it exists
  if (fields & FUJI_FIELD_DATA) {
    memcpy(&_lynx_packet[_lynx_len], buf, buf_length);
    _lynx_len += buf_length;
  }

  // Send the command (and data)
  r = fnio_send_buf(device, &_lynx_packet[0], _lynx_len);
  if (!r)
    return(false);

  // Get reply if one expected
  if (fields & FUJI_FIELD_REPLY) {
    r = fnio_recv_buf((char *) buf, &_lynx_len, buf_length);
    if ((!r) || (_lynx_len == 0))
      return(false);
  }
  // Get ACK from Fujinet that command succeeded
  else {
    r = fnio_recv_ack();
    if (!r)
      return(false);
  }

  return(true);
}
