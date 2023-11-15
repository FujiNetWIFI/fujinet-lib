#include <stdint.h>

#include <cbm.h>

#include "fujinet-io.h"
#include "fn_data.h"

uint8_t fn_io_get_wifi_status(void)
{
  uint8_t ws=0xFF;
  
  cbm_open(LFN,DEV,SAN,FUJICMD_GET_WIFISTATUS);
  cbm_read(LFN,&ws,sizeof(ws));
  cbm_close(LFN);
  return ws;

}
