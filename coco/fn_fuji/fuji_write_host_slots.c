#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Write host slots from FujiNet
 * @param h Destination pointer for host slot data.
 */
void fuji_write_host_slots(HostSlot *h)
{
  byte whscmd[2]={OP_FUJI,FUJICMD_WRITE_HOST_SLOTS};
  byte z=0;
  
  memset(h,0,256);
  
  while (!z)
    {
      dwwrite(whscmd,sizeof(whscmd));
      dwwrite((byte *)h,256);
    }
}
