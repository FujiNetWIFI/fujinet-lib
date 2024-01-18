#include <dw.h>
#include "fuji.h"

/**
 * @brief Get the currently configured SSID and passkey.
 * @param Pointer to a NetConfig struct to hold SSID and passkey.
 * @return TRUE if successful, FALSE if not.
 */
BOOL fuji_get_ssid(NetConfig *nc)
{
  byte cmd[2]={OP_FUJI,FUJICMD_GET_SSID};
  
  dwwrite(cmd,sizeof(cmd));
  return dwread((byte *)&nc, sizeof(nc));
}
