#include <fujinet-network.h>

FN_ERR network_set_username(const char *devicespec, const char *username)
{
  uint8_t nw_unit = network_unit(devicespec);


  // Payload is the bare username with no Nn: prefix
  if (!NETCALL_A1_A2_D(NETCMD_USERNAME, nw_unit, 0, 0, username,
                       NETWORK_FS_LEN(username)))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
