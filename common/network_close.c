#include <fujinet-network.h>

FN_ERR network_close(const char *devicespec)
{
  return !NETCALL(NETCMD_CLOSE, network_unit(devicespec));
}
