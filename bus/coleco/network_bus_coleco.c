#include "fujinet-bus-coleco.h"
#include "fujinet-commands.h"

/*
  Both directions are capped by the mailbox rather than by the caller. The
  common network_read()/network_write() loops in common/ already drive these
  by the count returned, so a short transfer here simply becomes another turn
  around their loop.
*/

size_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  if (length > FN_REPLY_MAX)
    length = FN_REPLY_MAX;

  if (!NETCALL_B12_RV(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length,
                      buffer, length))
    return 0;

  return fuji_bus_call_rlen;
}

size_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  /* One 16-bit parameter costs three stream bytes, so this is the largest
     payload that still fits inside FN_TX_MAX. */
  if (length > FN_TX_MAX - 3)
    length = FN_TX_MAX - 3;

  if (!NETCALL_B12_D(FUJICMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, length,
                     buffer, length))
    return 0;

  return length;
}
