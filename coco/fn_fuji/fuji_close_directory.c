#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Close open directory handle
 */
void fuji_close_directory(void)
{
  byte ccmd[2]={OP_FUJI,FUJICMD_CLOSE_DIRECTORY};

  dwwrite(ccmd,sizeof(ccmd));
}
