#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Get directory position of directory opened by fuji_open_directory()
 * @return integer containing current directory position (0-65535)
 */
unsigned int fuji_get_directory_position(void)
{
  byte gdpcmd[2]={OP_FUJI,FUJICMD_GET_DIRECTORY_POSITION};
  unsigned int pos=0;
  
  dwwrite(gdpcmd,sizeof(gdpcmd));
  dwread((byte *)&pos,sizeof(pos));

  return pos;
}
