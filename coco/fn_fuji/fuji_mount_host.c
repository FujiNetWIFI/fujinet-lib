#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Mount host slot specified by hs (0-3)
 * @param hs the Host slot to mount (0-3)
 */
void fuji_mount_host(byte hs)
{
  byte cmd[3] = {OP_FUJI,FUJICMD_MOUNT_HOST,0x00};

  cmd[2] = hs;

  dwwrite(&cmd, sizeof(cmd));
}
