#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fuji.h"

/**
 * @brief Read next directory entry in open directory
 * @param maxlen Maximum length of filename to return (longer names are center-ellipsized)
 * @param a if set to 128, return additional information (such as size and modified date)
 * @param s target string to return directory entry, must be l+1
 */
void fuji_read_directory(byte maxlen, byte a, char *s)
{
  BOOL z = FALSE;
  byte rcmd[4] = {OP_FUJI,FUJICMD_READ_DIR_ENTRY,0x00,0x00};
  
  if (a)
    maxlen += 10;

  rcmd[2]=maxlen;
  rcmd[3]=a;

  while (!z)
    {
      dwwrite(rcmd,sizeof(rcmd));
      z=dwread((byte *)s,maxlen);
    }
}
