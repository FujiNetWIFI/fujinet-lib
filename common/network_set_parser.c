#include <fujinet-network.h>

FN_ERR network_set_parser(const char *devicespec, uint8_t mode)
{
  uint8_t nw_unit = network_unit(devicespec);


  // Mode goes in both aux bytes: SIO/RS232 read aux2, unified buses read aux1
  if (!NETCALL_A1_A2(NETCMD_SET_PARSER, nw_unit, mode, mode))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
