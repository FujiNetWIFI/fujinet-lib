#include <fujinet-bus.h>
#include <fujinet-commands.h>

size_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
#if FUJI_NETWORK_READ_MAX
  if (length > FUJI_NETWORK_READ_MAX)
    length = FUJI_NETWORK_READ_MAX;
#endif
  if (!NETCALL_B12_RV(NETCMD_READ, device - FUJI_DEVICEID_NETWORK + 1,
                      length, buffer, length))
    return 0;
  return length;
}

size_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
#if FUJI_NETWORK_WRITE_MAX
  if (length > FUJI_NETWORK_WRITE_MAX)
    length = FUJI_NETWORK_WRITE_MAX;
#endif
  if (!NETCALL_B12_D(NETCMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1,
                     length, buffer, length))
    return 0;
  return length;
}
