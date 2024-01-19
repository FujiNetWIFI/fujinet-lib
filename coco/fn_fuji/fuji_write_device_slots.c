#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Write device slots from FujiNet
 * @param h Destination pointer for device slot data.
 */
void fuji_write_device_slots(DeviceSlot *d)
{
  byte wdscmd[2]={OP_FUJI,FUJICMD_WRITE_DEVICE_SLOTS};
  byte z=0;
  
  memset(d,0,152);
  
  while (!z)
    {
      dwwrite(wdscmd,sizeof(wdscmd));
      dwwrite((byte *)d,152);
    }
}
