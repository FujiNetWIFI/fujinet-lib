#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Ask FujiNet to scan for WiFi Networks
 * @return # of networks found.
 * @verbose This function will block until completion.
 */
byte fuji_scan_networks(void)
{
  byte n=0;
  byte cmd[2]={OP_FUJI,FUJICMD_SCAN_NETWORKS};
  byte z=0;
  
  while (!z)
    {
      dwwrite(cmd,sizeof(cmd));
      z = dwread(&n,1);      
    }
  
  return n;
}
