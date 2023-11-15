#include <stdint.h>
#include <cbm.h>
#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_get_device_slots(DeviceSlot *d)
{
  cbm_open(LFN, DEV, SAN, FUJICMD_READ_DEVICE_SLOTS);
  cbm_read(LFN, d, 152); // 38 * 4
  cbm_close(LFN);
}
