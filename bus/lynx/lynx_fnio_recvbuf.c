/**
 *  for Atari Lynx
*/

#include <lynx.h>
#include <serial.h>
#include <stdlib.h>
#include <stdio.h>
#include "lynxfnio.h"
#include "fujinet-commands.h"



bool fnio_recv_buf(char *buf, unsigned int *len, unsigned int maxlen)
{
  register unsigned short i;
  int t;
  char _r;
  uint8_t _ck;


  // reset error status
  _fn_error = FNIO_ERR_NONE;

  // Get first length byte
  t = _serial_get_loop();
  if (t < 0)
    return false;
  *len = t << 8;

  // Get second length byte
  t = _serial_get_loop();
  if (t < 0)
    return false;
  *len |= t & 0xFF;

  if (*len > maxlen)
    return false;

  if (*len > FNIO_TX_LEN_MAX) // no more than LEN_MAX bytes
    *len = FNIO_TX_LEN_MAX;

  // Now get the payload
  for (i=0; i<*len; ++i) {
    t = _serial_get_loop();
    if (t < 0)
      return false;
    buf[i] = t;
  }

  // Get the checksum
  t = _serial_get_loop();
  if (t < 0)
    return false;         // timeout

                          // checksum matches?
  _ck = _checksum(buf, *len);
  if (t == _ck) {
    ser_put(FUJICMD_ACK); // ACK
    ser_get(&_r);         // get reflected data
    return true;          // succes, checksum matches
  }

  _fn_error = FNIO_ERR_RECV_CHK;
  ser_put(FUJICMD_NAK);   // NACK, checksum bad
  ser_get(&_r);           // get reflected data
  *len = 0;               // return zero length
  return false;           // checksum bad
}
