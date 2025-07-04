#include "fujinet-fuji.h"

bool fuji_get_adapter_config_extended(AdapterConfigExtended *acx)
{
  if (!FUJICALL(FUJICMD_GET_ADAPTERCONFIG_EXTENDED))
    return false;

  fuji_bus_get(acx, sizeof(AdapterConfigExtended));
  return true;
}
