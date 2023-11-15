#include <stdint.h>
#include <string.h>
#include <cbm.h>

#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_get_ssid(NetConfig *net_config)
{
  memset(net_config, 0, sizeof(net_config));

  cbm_open(LFN,DEV,SAN,FUJICMD_GET_SSID);
  cbm_read(LFN,net_config,sizeof(NetConfig));
  cbm_close(LFN);  
  
}
