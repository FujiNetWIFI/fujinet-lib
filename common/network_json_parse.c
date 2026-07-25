#include <fujinet-network.h>

FN_ERR network_json_parse(const char *devicespec)
{
  uint8_t err = 0;
  uint8_t nw_unit = network_unit(devicespec);


  // Atari and RS232 use aux2, CoCo and Apple2 use aux1
  if (!NETCALL_A1_A2(FUJICMD_JSON_MODE, nw_unit, 1, 1))
    return FN_ERR_IO_ERROR;

  if (!NETCALL(FUJICMD_PARSE, nw_unit))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
