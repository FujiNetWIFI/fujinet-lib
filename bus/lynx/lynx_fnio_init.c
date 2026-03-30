/**
 *  for Atari Lynx
 */

#include <6502.h>
#include <lynx.h>
#include <serial.h>
#include <stdlib.h>
#include <stdio.h>
#include "lynxfnio.h"



unsigned char fnio_init(void)
{ 
  struct ser_params params = {
    SER_BAUD_62500,
    SER_BITS_8,
    SER_STOP_1,
    SER_PAR_ODD,
    SER_HS_NONE
  };

  ser_install(lynx_comlynx_ser); // This will activate the ComLynx
  CLI();

  // do open
  return(!ser_open(&params));
}
