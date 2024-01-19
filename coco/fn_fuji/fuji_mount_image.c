#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Mount drive slot specified by ds (0-3) with mode m
 * @param ds the drive slot to mount (0-3)
 * @param m The mode to use (0=r/o 2=r/w)
 */
void fuji_mount_image(byte ds, byte m)
{
  byte cmd[4] = {OP_FUJI,FUJICMD_MOUNT_IMAGE,0x00,0x00};

  cmd[2] = ds;
  cmd[3] = m;

  dwwrite(&cmd, sizeof(cmd));
}
