#include "fujinet-fuji.h"

bool fuji_get_device_slots(DeviceSlot *d, size_t count)
{
  if (!FUJICALL(FUJICMD_READ_DEVICE_SLOTS))
    return false;
  if (!fuji_bus_get(d, sizeof(DeviceSlot) * count))
    return false;
  return true;
}
