#include "fujinet-bus-adam.h"
#include "fujinet-network.h"

#include <stdio.h> // debug

int16_t network_read_nb_adam(const char *devicespec, void *buf, uint16_t len)
{
  uint8_t device, nw_unit;
  uint8_t status;
  DCB *dcb;


  nw_unit = network_unit(devicespec);
  device = fuji_remap_device(nw_unit + FUJI_DEVICEID_NETWORK - 1);
  if (!device)
    return 0;
  dcb = dcb_find(device);
  if (!dcb)
    return 0;

  status = dcb_io(dcb, DCB_COMMAND_READ, buf, len);
  if (status != DCB_STATUS_FINISH)
    return 0;
  return dcb->len;
}
