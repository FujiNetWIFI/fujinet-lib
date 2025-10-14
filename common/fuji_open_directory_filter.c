#include "fujinet-fuji.h"
#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else /* ! _CMOC_VERSION_ */
#include <string.h>
#endif /* _CMOC_VERSION_ */

static char buffer[256];

bool fuji_open_directory_filter(uint8_t hostSlot, const char *path, const char *filter)
{
  const char *ptr = path;
  size_t len;


  if (filter[0]) {
    strcpy(buffer, ptr);
    len = strlen(buffer);
    strcpy(&buffer[len + 1], filter);
    ptr = buffer;
  }
  return fuji_open_directory(hostSlot, ptr);
}
