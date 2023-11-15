#include <stdint.h>
#include <string.h>
#include <cbm.h>

#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_put_host_slots(HostSlot *h)
{
  memset(response, 0, sizeof(response));

  response[0] = FUJICMD_WRITE_HOST_SLOTS;

  memcpy(&response[1], h, 8 * sizeof(HostSlot));

  cbm_open(LFN, DEV, SAN, response);
  cbm_close(LFN);
}
