#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Unmount disk image in host slot hs
 * @param hs Host slot to unmount (0-7)
 */
void fuji_unmount_host_slot(byte hs)
{
  byte uhscmd[3]={OP_FUJI,FUJICMD_UNMOUNT_HOST,0x00};

  uhscmd[2]=hs;

  dwwrite(uhscmd,sizeof(uhscmd));
}
