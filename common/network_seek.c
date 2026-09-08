#include <fujinet-network.h>

#ifdef DEVCALL_C1234
FN_ERR network_seek(const char *devicespec, uint32_t pos)
{
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_C1234(NETCMD_SEEK, nw_unit, pos))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
#endif // DEVCALL_C1234
