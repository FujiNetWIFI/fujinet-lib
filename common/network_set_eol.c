#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <string.h>
#endif /* _CMOC_VERSION_ */

FN_ERR network_set_eol(const char *devicespec, const char *eol)
{
  return NETCALL_VLD(NETCMD_SET_EOL, network_unit(devicespec),
                     eol, strlen(eol)) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}
