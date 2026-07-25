#include <fujinet-network.h>

FN_ERR network_accept(const char *devicespec)
{
  uint8_t nw_unit;
  bool success;


  nw_unit = network_unit(devicespec);
  success = NETCALL(FUJICMD_ACCEPT, nw_unit);
  if (!success)
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
