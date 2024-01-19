#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Get FujiNet Adapter Config and place in AdapterConfig structure pointed to by ac
 * @param ac Pointer to an allocated AdapterConfig structure
 */
void fuji_get_adapter_config(AdapterConfig *ac)
{
  BOOL z=FALSE;
  byte gacmd[2]={OP_FUJI,FUJICMD_GET_ADAPTERCONFIG};

  while (!z)
    {
      dwwrite(gacmd,sizeof(gacmd));
      dwread((byte *)&ac,sizeof(AdapterConfig));
    }
}
