#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Set WiFi SSID and Passphrase, and start connection.
 * @param nc Pointer to NetConfig struct with SSID and passkey
 * @return TRUE if successful, FALSE if failed.
 */
BOOL fuji_set_ssid(NetConfig *nc)
{
  struct _setssid_cmd
  {
    byte op;
    byte c;
    byte ssid[33];
    byte password[64];
  } cmd;

  cmd.op = OP_FUJI;
  cmd.c = FUJICMD_SET_SSID;

  memcpy(cmd.ssid,nc->ssid,33);
  memcpy(cmd.password,nc->password,64);

  dwwrite((byte *)&cmd, sizeof(cmd));

  return TRUE;
}
