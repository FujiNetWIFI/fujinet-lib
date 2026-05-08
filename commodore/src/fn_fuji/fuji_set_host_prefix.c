#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

#define PL_BUF_SIZE 256

bool fuji_set_host_prefix(uint8_t hs, char *prefix)
{
  uint8_t pl[PL_BUF_SIZE];
  uint16_t pl_len;

  pl_len = strlen(prefix) + 3 + 1;

  pl[0] = hs;
  strcpy((char *) &pl[1], prefix);
  pl[pl_len - 1] = '\0';

  return open_close_data(FUJICMD_SET_HOST_PREFIX, true, pl_len, pl);
}
