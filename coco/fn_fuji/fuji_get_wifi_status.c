#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Return status of WiFi adapter
 * @return WIFI Status byte
 */
byte fuji_get_wifi_status(void)
{
  byte cmd[2]={OP_FUJI,FUJICMD_GET_WIFISTATUS};
  byte z=0;
  byte s=0;
  
  while (!z)
    {
      dwwrite(cmd,sizeof(cmd));
      z=dwread(&s,1);
    }

  return s;
}
