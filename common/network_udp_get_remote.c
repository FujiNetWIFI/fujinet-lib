#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <string.h>
#endif /* _CMOC_VERSION_ */

FN_ERR network_udp_get_remote(const char *devicespec, char *buf, uint16_t len)
{
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_RV(NETCMD_GET_REMOTE, nw_unit, buf, len))
    return FN_ERR_IO_ERROR;

  // Reply is "ip:port" followed by an EOL and padding on fixed-length buses
  buf[len - 1] = '\0';
  network_json_strip_newlines(buf, strlen(buf));
  return FN_ERR_OK;
}
