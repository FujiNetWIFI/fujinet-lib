#include <fujinet-network.h>

FN_ERR network_set_password(const char *devicespec, const char *password)
{
  uint8_t nw_unit = network_unit(devicespec);


  // Payload is the bare password with no Nn: prefix
  if (!NETCALL_A1_A2_D(NETCMD_PASSWORD, nw_unit, 0, 0, password,
                       NETWORK_FS_LEN(password)))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
