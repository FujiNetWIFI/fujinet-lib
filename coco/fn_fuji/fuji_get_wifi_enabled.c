#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Return whether WIFI is enabled (1) or disabled (0)
 * @return 0 = wifi disabled, 1 = wifi enabled
 */
BOOL fuji_get_wifi_enabled(void)
{
  byte wecmd[2]={OP_FUJI,FUJICMD_GET_WIFI_ENABLED};
  BOOL z=FALSE;
  BOOL e=FALSE;
  
  while (!z)
    {
      dwwrite(wecmd,sizeof(wecmd));
      z = dwread(&e,1);
    }

  return e;
}
