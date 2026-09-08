#include <fujinet-network.h>

FN_ERR network_sgml_parse(const char *devicespec)
{
  uint8_t nw_unit = network_unit(devicespec);
  FN_ERR err = network_set_parser(devicespec, PARSER_SGML);


  if (err != FN_ERR_OK)
    return err;

  if (!NETCALL(NETCMD_PARSE, nw_unit))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
