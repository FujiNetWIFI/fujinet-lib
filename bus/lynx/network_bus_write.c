#include <fujinet-bus.h>
#include <fujinet-commands.h>
#include "lynxfnio.h"

/* One packet byte is the command; network_write() sends the rest. */
size_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  if (length > FNIO_TX_LEN_MAX - 1)
    length = FNIO_TX_LEN_MAX - 1;
  if (!NETCALL_D(NETCMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, buffer, length))
    return 0;
  return length;
}
