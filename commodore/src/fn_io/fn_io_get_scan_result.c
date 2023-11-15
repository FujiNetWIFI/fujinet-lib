#include <stdint.h>
#include <string.h>

#include <cbm.h>

#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
  memset(ssid_info, 0, sizeof(ssid_info));

  cbm_open(LFN, DEV, SAN, FUJICMD_GET_SCAN_RESULT);
  cbm_read(LFN, ssid_info, sizeof(SSIDInfo));
  cbm_close(LFN);
}
