#include <stdint.h>
#include <cbm.h>
#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_get_host_slots(HostSlot *h)
{
  cbm_open(LFN, DEV, SAN, FUJICMD_READ_HOST_SLOTS);
  cbm_read(LFN, h, 256);
  cbm_close(LFN);
}
