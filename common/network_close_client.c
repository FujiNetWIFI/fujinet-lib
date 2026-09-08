#include <fujinet-network.h>

FN_ERR network_close_client(const char *devicespec)
{
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL(NETCMD_CLOSE_CLIENT, nw_unit))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
