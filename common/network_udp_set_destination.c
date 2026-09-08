#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <string.h>
#endif /* _CMOC_VERSION_ */

FN_ERR network_udp_set_destination(const char *dest_spec)
{
  uint8_t nw_unit = network_unit(dest_spec);


  if (!NETCALL_VLD(NETCMD_SET_DESTINATION, nw_unit, dest_spec, strlen(dest_spec)))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
