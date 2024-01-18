#include <cmoc.h>
#include <coco.h>
#include <dw.h>

/**
 * @brief Ask FujiNet to scan for WiFi Networks
 * @return # of networks found.
 * @verbose This function will block until completion.
 */
byte fuji_scan_networks(void)
{
  byte n=0;
  byte cmd[2]={0xE2,0xFD};
  byte z=0;
  
  while (!z)
    {
      dwwrite(cmd,sizeof(cmd));
      z = dwread(&n,1);      
    }
  
  return n;
}
