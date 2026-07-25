#include "network.h"

#include <fujinet-network.h>

NetworkStatus nw_status;

FN_ERR network_status(const char *devicespec, uint16_t *avail, uint8_t *status, uint8_t *err)
{
  FN_ERR stat_err = network_unit_status(network_unit(devicespec), &nw_status);

  if (stat_err != FN_ERR_OK)
    return stat_err;

  *avail = nw_status.avail;
  *status = nw_status.status;
  *err = nw_status.errcode;

  return FN_ERR_OK;
}
