#include <fujinet-network.h>

FN_ERR network_write(const char *devicespec, const void *buf, uint16_t len)
{
  uint8_t nw_unit;
  uint16_t wlen;
  uint8_t *ubuf = (uint8_t *) buf;


  nw_unit = network_unit(devicespec);
  while (len) {
    wlen = network_bus_write(FUJI_DEVICEID_NETWORK + nw_unit - 1, ubuf, len);
    if (len && !wlen)
      return FN_ERR_IO_ERROR;
    len -= wlen;
    ubuf += wlen;
  }

  return FN_ERR_OK;
}
