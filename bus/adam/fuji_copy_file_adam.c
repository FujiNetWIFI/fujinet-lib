#include "fujinet-bus-adam.h"
#include "fujinet-fuji.h"

/* AdamNet completes a write on delivery, so wait for the copy to finish. */
bool fuji_copy_file_adam(uint8_t src_slot, uint8_t dest_slot,
			 const char *copy_spec)
{
  DCB *dcb;


  if (!FUJICALL_A1_A2_D(FUJICMD_COPY_FILE, src_slot, dest_slot, copy_spec,
			MAX_FILENAME_LEN))
    return false;

  dcb = dcb_find(fuji_remap_device(FUJI_DEVICEID_FUJINET));
  if (!dcb)
    return false;

  return bus_ready(dcb, COPY_RETRIES);
}
