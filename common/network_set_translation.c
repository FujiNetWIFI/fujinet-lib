#include <fujinet-network.h>

FN_ERR network_set_translation(const char *devicespec, uint8_t trans)
{
  uint8_t nw_unit = network_unit(devicespec);


  // Mode goes in both aux bytes: SIO reads aux2, unified buses read aux1.
  // On SIO the value is sticky: it is ORed into aux2 of subsequent opens,
  // except 0xFF which makes opens ignore aux2 entirely, and it is never
  // applied to directory opens (aux2 is a format code there).
  if (!NETCALL_A1_A2(NETCMD_TRANSLATION, nw_unit, trans, trans))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
