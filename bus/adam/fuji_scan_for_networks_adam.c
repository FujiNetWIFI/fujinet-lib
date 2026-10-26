#include "fujinet-bus-adam.h"
#include "fujinet-fuji.h"

/* The send waits out the scan; a combined send and read would time out. */
bool fuji_scan_for_networks_adam(uint8_t *count)
{
  DCB *dcb;


  if (!FUJICALL(FUJICMD_SCAN_NETWORKS))
    return false;

  dcb = dcb_find(fuji_remap_device(FUJI_DEVICEID_FUJINET));
  if (!dcb)
    return false;

  return dcb_io(dcb, DCB_COMMAND_READ, count, 1, MAX_RETRIES) == DCB_STATUS_FINISH;
}
