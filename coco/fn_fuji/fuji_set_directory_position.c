#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Set directory position of directory opened by fuji_open_directory()
 * @param pos integer containing new directory position (0-65535)
 */
void fuji_set_directory_position(unsigned int pos)
{
  byte gdpcmd[4]={OP_FUJI,FUJICMD_SET_DIRECTORY_POSITION,0,0};

  gdpcmd[2]=(byte)(pos>>8);
  gdpcmd[3]=(byte)pos&0xFF;
  
  dwwrite(gdpcmd,sizeof(gdpcmd));
}

