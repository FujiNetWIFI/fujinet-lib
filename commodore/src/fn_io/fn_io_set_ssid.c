#include <stdint.h>
#include <string.h>

#include <cbm.h>

#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_set_ssid(NetConfig *nc)
{
  char c[98];

  c[0] = 0xFB; // FUJICMD_SET_SSID
  memcpy(&c[1], nc, sizeof(NetConfig));

  cbm_open(LFN, DEV, SAN, c);
  cbm_close(LFN);
}
