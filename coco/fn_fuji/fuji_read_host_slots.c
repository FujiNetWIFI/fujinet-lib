#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Read host slots from FujiNet
 * @param h Destination pointer for host slot data.
 */
void fuji_read_host_slots(HostSlot *h)
{
  byte ghscmd[2]={OP_FUJI,FUJICMD_READ_HOST_SLOTS};
  byte z=0;
  
  memset(h,0,256);
  
  while (!z)
    {
      dwwrite(ghscmd,sizeof(ghscmd));
      z = dwread((byte *)h,256);
    }
}
