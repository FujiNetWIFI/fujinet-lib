#include "network.h"

#include <fujinet-network.h>
#include <fujinet-const.h>

FN_ERR network_open(const char *devicespec, uint8_t mode, uint8_t trans)
{
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_A1_A2_D(FUJICMD_OPEN, nw_unit, mode, trans, devicespec, NETWORK_OPEN_LEN))
    return FN_ERR_IO_ERROR;

#if 0
  // FIXME - if this is a POST open then calling status will try do do the POST with no data!
  if (!network_unit_status(network_unit(devicespec), &nw_status))
    return FN_ERR_IO_ERROR;

  /* We haven't even read the file yet, it's not EOF */
  if (nw_status.errcode == NETWORK_ERROR_END_OF_FILE)
    nw_status.errcode = NETWORK_SUCCESS;

  if (nw_status.errcode > NETWORK_SUCCESS && !nw_status.avail)
    return nw_status.errcode;
#endif

  return FN_ERR_OK;
}
