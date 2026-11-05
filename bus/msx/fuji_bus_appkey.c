#include <fujinet-const.h>
#include <fujinet-int.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>
#include <fujinet-appkey.h>
#include <string.h>

// Assumes the reply is the raw key, as AppKeyMixin::appkey_read() sends it
bool fuji_bus_appkey_read(void *string, uint16_t *length)
{
  uint8_t *s = (uint8_t *) string;
  uint16_t n = 0;

  memset(s, 0, MAX_APPKEY_LEN);
  if (!FUJICALL_RV(FUJICMD_READ_APPKEY, s, MAX_APPKEY_LEN))
    return false;
  while (n < MAX_APPKEY_LEN && s[n])
    n++;
  *length = n;
  return true;
}

bool fuji_bus_appkey_write(const void *string, uint16_t length)
{
  return FUJICALL_D(FUJICMD_WRITE_APPKEY, string, length);
}
