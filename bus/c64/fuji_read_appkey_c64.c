#include "fujinet-fuji.h"
#include "fujinet-cbm.h"

// Set up by init_appkey(): creator id, app id, mode.
// (Declared here as a plain byte array instead of the old
// "extern void appkey; &appkey" trick, which took the address of a
// mis-declared function just to get a pointer to data.)
extern uint8_t appkey[6];

extern void init_appkey(uint8_t key_id, uint8_t mode);

static bool fuji_check_status(void)
{
  uint8_t status_cmd[2] = { OPCODE_NO_PAYLOAD, FUJICMD_STATUS };
  FNStatus status;


  if (cbm_write(CBM_CMD_CHANNEL, status_cmd, 2) != 2)
    return false;

  cbm_read(CBM_CMD_CHANNEL, status.raw, sizeof(FNStatus));
  return status.value.error == 0;
}

bool fuji_read_appkey_c64(uint8_t key_id, uint16_t *count, uint8_t *data)
{
  uint8_t open_cmd[2] = { OPCODE_HAS_PAYLOAD, FUJICMD_OPEN_APPKEY };
  uint8_t read_cmd[2] = { OPCODE_NO_PAYLOAD,  FUJICMD_READ_APPKEY };
  int bytes_read;
  bool success = false;


  *count = 0;

  if (ak_creator_id == 0)
    return false;

  init_appkey(key_id, 0);

  // Open the FujiNet command channel and send the "open appkey" opcode.
  if (fuji_cbm_open(CBM_CMD_CHANNEL, CBM_DEV_FUJI, CBM_CMD_CHANNEL, 2, open_cmd) != 0)
    return false;

  if (cbm_write(CBM_CMD_CHANNEL, appkey, sizeof(appkey)) != sizeof(appkey))
    goto cleanup;

  if (!fuji_check_status())
    goto cleanup;

  if (cbm_write(CBM_CMD_CHANNEL, read_cmd, 2) != 2)
    goto cleanup;

  bytes_read = cbm_read(CBM_CMD_CHANNEL, data, 64); // TODO: key sizes overhaul
  *count = (bytes_read >= 0) ? bytes_read : 0;

  success = fuji_check_status();

 cleanup:
  cbm_close(CBM_CMD_CHANNEL);
  return success;
}
