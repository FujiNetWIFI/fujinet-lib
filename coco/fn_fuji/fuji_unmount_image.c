#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Unmount disk image in device slot ds
 * @param ds Device slot to unmount (0-3)
 */
void fuji_unmount_disk_image(byte ds)
{
  byte udicmd[3]={OP_FUJI,FUJICMD_UNMOUNT_IMAGE,0x00};

  udicmd[2]=ds;

  dwwrite(udicmd,sizeof(udicmd));
}
