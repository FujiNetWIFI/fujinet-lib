#include "fujinet-fuji.h"
#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else /* ! _CMOC_VERSION_ */
#include <string.h>
#endif /* _CMOC_VERSION_ */

static char buffer[256];

bool fuji_open_directory_filter(uint8_t hostSlot, const char *path, const char *filter)
{
  size_t len;


  strcpy(buffer, path);
  len = strlen(buffer);
  /* A second NUL is an empty filter; anything else after the path is sent as one. */
  buffer[len + 1] = '\0';
  if (filter && filter[0])
    strcpy(&buffer[len + 1], filter);
  return fuji_open_directory(hostSlot, buffer);
}
