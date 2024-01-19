#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Return scan result, after fuji_scan_networks
 * @return TRUE if valid, false if INVALID (e.g. over index)
 * @verbose This function will block until completion.
 */
byte fuji_get_scan_result(byte i, SSIDInfo *s)
{
  byte n=0;
  byte cmd[3]={OP_NET,FUJICMD_GET_SCAN_RESULT,0x00};
  byte z=0;

  cmd[2] = i;
  
  while (!z)
    {
      dwwrite(cmd,sizeof(cmd));
      z = dwread((byte *)&s,sizeof(SSIDInfo));
    }
  
  return z;
}
