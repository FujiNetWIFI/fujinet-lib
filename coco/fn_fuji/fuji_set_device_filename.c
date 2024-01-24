#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Return device filename to string pointer s (needs to be 256 bytes!)
 * @param ds the device slot to return (0-3)
 * @param s Destination pointer
 */
void fuji_set_device_filename(byte ds, char *s)
{
  byte sdfcmd[259];

  memset(sdfcmd,0,sizeof(sdfcmd));

  sdfcmd[0] = OP_FUJI;
  sdfcmd[1] = FUJICMD_SET_DEVICE_FULLPATH;
  sdfcmd[2] = ds;
  strcpy((char *)&sdfcmd[3],s);
  
  dwwrite(sdfcmd,sizeof(sdfcmd));
  dwread((byte *)s,256);
}
