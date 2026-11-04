#include "fujinet-bus-coleco.h"
#include "fujinet-commands.h"

/* Transfers are capped by the mailbox; the common loops turn a short count into another pass. */

size_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  if (length > FN_REPLY_MAX)
    length = FN_REPLY_MAX;

  if (!NETCALL_B12_RV(NETCMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length,
                      buffer, length))
    return 0;

  return fuji_bus_call_rlen;
}

size_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  /* One 16-bit parameter costs three stream bytes. */
  if (length > FN_TX_MAX - 3)
    length = FN_TX_MAX - 3;

  if (!NETCALL_B12_D(NETCMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, length,
                     buffer, length))
    return 0;

  return length;
}
