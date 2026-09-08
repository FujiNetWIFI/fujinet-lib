#include <fujinet-network.h>

FN_ERR network_set_parameter(const char *devicespec, uint8_t param, uint8_t value)
{
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_A1_A2(NETCMD_SET_PARAMETER, nw_unit, param, value))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
