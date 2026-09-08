#include <fujinet-network.h>

FN_ERR network_tell_common(const char *devicespec, uint32_t *pos)
{
  uint8_t buf[4];
  uint8_t nw_unit = network_unit(devicespec);


  /* RS232 and unified buses reply with 4 bytes little-endian */
  if (!NETCALL_RV(NETCMD_TELL, nw_unit, buf, 4))
    return FN_ERR_IO_ERROR;
  *pos = ((uint32_t) buf[3] << 24) | ((uint32_t) buf[2] << 16)
    | ((uint16_t) buf[1] << 8) | buf[0];
  return FN_ERR_OK;
}
