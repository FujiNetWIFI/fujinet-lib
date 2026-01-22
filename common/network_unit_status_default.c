#include "network_unit_status.h"
#include <fujinet-commands.h>
#include <fujinet-bus.h>

/* Not used on C64, see bus/c64/network_unit_status.c */

FN_ERR network_unit_status(uint8_t unit, NetworkStatus *nw_status)
{
  // Inverting the bool converts true to FN_ERR_OK
  return !NETCALL_RV(FUJICMD_STATUS, unit, nw_status, sizeof(*nw_status));
}
