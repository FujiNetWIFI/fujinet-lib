#include "fujinet-fuji.h"
#include "fujinet-bus.h"

bool fuji_get_adapter_config_extended(AdapterConfigExtended *acx)
{
  if (!transaction_call(FUJICMD_GET_ADAPTERCONFIG_EXTENDED))
    return false;

  transaction_get(acx, sizeof(AdapterConfigExtended));
  return true;
}
