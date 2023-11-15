#include <stdint.h>
#include <string.h>

#include <cbm.h>

#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_get_adapter_config(AdapterConfig *ac)
{
  memset(ac, 0, sizeof(AdapterConfig));

  cbm_open(LFN, DEV, SAN, FUJICMD_GET_ADAPTERCONFIG);
  cbm_read(LFN, ac, sizeof(AdapterConfig));
  cbm_close(LFN);
}
