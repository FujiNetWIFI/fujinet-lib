#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Return device filename to string pointer s (needs to be 256 bytes!)
 * @param ds the device slot to return (0-3)
 * @param s Destination pointer
 */
void fuji_get_device_filename(byte ds, char *s)
{
  byte gdfcmd[3]={OP_FUJI,FUJICMD_GET_DEVICE_FULLPATH,0x00};

  gdfcmd[2] = ds;

  dwwrite(gdfcmd,sizeof(gdfcmd));
  dwread((byte *)s,256);
}
