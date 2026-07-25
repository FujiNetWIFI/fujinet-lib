#include "network.h"

#include <fujinet-network.h>

#if !defined(__ADAM__) && !defined(__COLECOADAM__)
int16_t network_read_nb(const char *devicespec, void *buf, uint16_t len)
{
  uint8_t nw_unit;
  FN_ERR err;


  nw_unit = network_unit(devicespec);

  do {
    // Check how many bytes are available
    err = network_unit_status(nw_unit, &nw_status);
    if (err != FN_ERR_OK)
      return -err;

    if (nw_status.errcode > NETWORK_SUCCESS && !nw_status.avail)
      return 0;
  } while (!nw_status.avail);

  if (len > nw_status.avail)
    len = nw_status.avail;

  return network_bus_read(FUJI_DEVICEID_NETWORK + nw_unit - 1, buf, len);
}
#endif /* ! (__ADAM__ || __COLECOADAM__) */
