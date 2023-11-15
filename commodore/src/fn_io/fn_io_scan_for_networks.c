#include <stdint.h>
#include <cbm.h>

#include "fujinet-io.h"
#include "fn_data.h"

uint8_t fn_io_scan_for_networks(void)
{
  uint8_t n;
  
  cbm_open(LFN,DEV,SAN,FUJICMD_SCAN_NETWORKS);
  cbm_read(LFN,&n,sizeof(n));
  cbm_close(LFN);

  return n;
}
