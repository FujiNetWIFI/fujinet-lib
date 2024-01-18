#include <dw.h>
#include "fuji.h"

/**
 * @brief Reset the FujiNet
 */
void fuji_reset(void)
{
  byte cmd[2]={OP_FUJI,FUJICMD_RESET};

  dwwrite(cmd,sizeof(cmd));
}
