#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Open directory connection to host slot hs, path p, and filter by f
 * @param hs Host slot to open (0-7)
 * @param p Path to open
 * @param f Filter by wildcard
 */
void fuji_open_directory(byte hs, char *p, char *f)
{
  struct _open_dir_cmd
  {
    byte op;
    byte cmd;
    byte hs;
    char path_with_filter[256];
  } odc;

  memset(&odc,0,sizeof(odc));
  
  odc.op = OP_FUJI;
  odc.cmd = FUJICMD_OPEN_DIRECTORY;
  odc.hs = hs;

  strcpy(odc.path_with_filter,p);
  strcpy(&odc.path_with_filter[strlen(odc.path_with_filter)+1],f);

  dwwrite((byte *)&odc,sizeof(odc));
}
