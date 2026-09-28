#include "fujinet-bus-adam.h"
#include <fujinet-network.h>
#include <string.h>

int16_t network_json_query_adam(const char *devicespec, const char *query, char *buffer)
{
  uint8_t nw_unit = network_unit(devicespec);
  uint8_t device;
  DCB *dcb;


  if (!NETCALL_D(NETCMD_QUERY, nw_unit, query, strlen(query)))
    return -FN_ERR_IO_ERROR;

  // A bare read takes the whole query result, whatever its length.
  device = fuji_remap_device(nw_unit + FUJI_DEVICEID_NETWORK - 1);
  if (!device)
    return 0;
  dcb = dcb_find(device);
  if (!dcb)
    return 0;
  if (dcb_io(dcb, DCB_COMMAND_READ, buffer, MAX_ADAM_PACKET_REPLY, MAX_RETRIES)
      != DCB_STATUS_FINISH)
    return 0;

  return network_json_strip_newlines(buffer, dcb->len);
}
