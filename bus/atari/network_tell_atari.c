#include <fujinet-network.h>

FN_ERR network_tell_atari(const char *devicespec, uint32_t *pos)
{
  uint8_t buf[4];
  uint8_t nw_unit = network_unit(devicespec);


  /* SIO replies with a 3 byte little-endian position */
  if (!NETCALL_RV(NETCMD_TELL, nw_unit, buf, 3))
    return FN_ERR_IO_ERROR;
  *pos = ((uint32_t) buf[2] << 16) | ((uint16_t) buf[1] << 8) | buf[0];
  return FN_ERR_OK;
}
