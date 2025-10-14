#include "fujinet-fuji.h"

bool fuji_get_wifi_enabled(void)
{
  char buf[1];


  FUJICALL_RV(FUJICMD_GET_WIFI_ENABLED, buf, 1);
  return (bool) buf[0];
}
