#include "fujinet-fuji.h"

// Open Watcom can't do far pointers in a function declaration
static char c;

bool fuji_get_wifi_enabled(void)
{
  FUJICALL_RV(FUJICMD_GET_WIFI_ENABLED, &c, 1);
  return (bool) c;
}
