#include "fujinet-bus-adam.h"
#include "fujinet-fuji.h"

/* AdamNet completes a write on delivery, so wait for the scan to finish. */
bool fuji_scan_for_networks_adam(uint8_t *count)
{
  DCB *dcb;


  if (!FUJICALL(FUJICMD_SCAN_NETWORKS))
    return false;

  dcb = dcb_find(fuji_remap_device(FUJI_DEVICEID_FUJINET));
  if (!dcb)
    return false;

  if (!bus_ready(dcb, LONG_RETRIES))
    return false;

  return dcb_io(dcb, DCB_COMMAND_READ, count, 1, MAX_RETRIES) == DCB_STATUS_FINISH;
}
