#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Read device slots from FujiNet
 * @param h Destination pointer for device slot data.
 */
void fuji_read_device_slots(DeviceSlot *d)
{
  byte ghscmd[2]={OP_FUJI,FUJICMD_READ_DEVICE_SLOTS};
  byte z=0;
  
  memset(d,0,256);
  
  while (!z)
    {
      dwwrite(ghscmd,sizeof(ghscmd));
      z = dwread((byte *)d,152);
    }
}
