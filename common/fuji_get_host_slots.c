#include "fujinet-fuji.h"

bool fuji_get_host_slots(HostSlot *h, size_t count)
{
  if (!FUJICALL(FUJICMD_READ_HOST_SLOTS))
    return false;
  if (!fuji_bus_get(h, sizeof(HostSlot) * count))
    return false;
  return true;
}
