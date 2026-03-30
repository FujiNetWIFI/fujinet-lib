/**
 *  for Atari Lynx
 *
 */

#include <lynx.h>
#include <serial.h>
#include <stdlib.h>
#include <stdio.h>
#include "lynxfnio.h"



unsigned char fnio_done(void)
{
  return ser_close();
}
