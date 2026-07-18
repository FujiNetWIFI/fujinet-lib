#include <fujinet-network.h>

FN_ERR network_set_eol(const char *devicespec, const char *eol)
{
  uint8_t nw_unit;


  nw_unit = network_unit(devicespec);
  /* Currently can only send a 2 byte string since it's being sent as aux bytes */
  if (!NETCALL_A1_A2(NETCMD_SET_EOL, nw_unit, eol[0], eol[1]))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
