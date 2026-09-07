#include <fujinet-bus.h>
#include <fujinet-commands.h>

#include "lynxfnio.h"

/* Return the length actually received, which may be less than requested. */
size_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  if (!NETCALL_RV(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, buffer, length))
    return 0;
  return _lynx_len;
}
