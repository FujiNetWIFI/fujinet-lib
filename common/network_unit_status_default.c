#include "network_unit_status.h"
#include <fujinet-commands.h>
#include <fujinet-bus.h>

/* Not used on C64, see bus/c64/network_unit_status.c */

uint8_t network_unit_status(uint8_t unit, NetworkStatus *nw_status)
{
  return NETCALL_RV(FUJICMD_STATUS, unit, nw_status, sizeof(*nw_status));
}
