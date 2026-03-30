/**
 *  for Atari Lynx
 *
 * @brief I/O routines
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 */

#include <lynx.h>
#include <serial.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "lynxfnio.h"
#include "fujinet-commands.h"



/**
 * @brief send packet to FujiNet device
 * @param dev The device # 0x00 - 0x0F
 * @param buf The buffer to send
 * @param len Length of buffer to send
 * @return 1 on ACK, 0 on NAK
 */
unsigned char fnio_send_buf(unsigned char dev, char *buf, unsigned int len)
{
  register unsigned short i;
  int ret;
  char _r;
  uint8_t _ck;


  // reset error status
  _fn_error = FNIO_ERR_NONE;

  // Calculate checksum on buffer
  _ck = _checksum(buf, len);

  // Send the Device and Length
  ser_put(dev);
  ser_put(len >> 8);
  ser_put(len & 0xFF);
  ser_get(&_r);           // get rid of reflected data
  ser_get(&_r);
  ser_get(&_r);

  // send the payload
  for (i=0; i<len; ++i) {
    //while (ser_put(buf[i]) != SER_ERR_OVERFLOW);        // handle if we overflowed the TX buffer
    ser_put(buf[i]);
    ser_get(&_r);         // get rid of reflected data we just sent
  }

  // send the checksum
  ser_put(_ck);
  ser_get(&_r);           // get rid of reflected data

  // Get response
  ret = _serial_get_loop();
  if (ret < 0)
    return false;

  // ret contains our response, ACK or NACK
  if (ret == FUJICMD_ACK)
    return true;
  else {
    _fn_error = FNIO_ERR_SEND_CHK;
    return false;
  }
}
