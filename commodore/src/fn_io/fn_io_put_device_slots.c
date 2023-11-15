#include <stdint.h>
#include <string.h>
#include <cbm.h>
#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_put_device_slots(DeviceSlot *d)
{
  memset(response, 0, sizeof(response));

  response[0] = FUJICMD_WRITE_DEVICE_SLOTS;

  memcpy(&response[1], d, 4 * sizeof(DeviceSlot)); // 4 * 38

  cbm_open(LFN, DEV, SAN, response);
  cbm_close(LFN);
}
