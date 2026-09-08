#include <fujinet-network.h>

// Atari can only has 16 bits of parameter, send position as data
FN_ERR network_seek_atari(const char *devicespec, uint32_t pos)
{
  uint8_t buf[3];
  uint8_t nw_unit = network_unit(devicespec);


  /* SIO takes the offset as a 3 byte little-endian data frame */
  buf[0] = pos & 0xFF;
  buf[1] = (pos >> 8) & 0xFF;
  buf[2] = (pos >> 16) & 0xFF;
  if (!NETCALL_D(NETCMD_SEEK, nw_unit, buf, sizeof(buf)))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
