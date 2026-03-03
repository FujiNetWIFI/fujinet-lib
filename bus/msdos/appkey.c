#include "fujinet-const.h"
#include "fujinet-int.h"
#include "fujinet-bus.h"
#include "fujinet-commands.h"
#include <string.h>

typedef struct {
  uint16_t length;
  uint8_t data[MAX_APPKEY_LEN];
} FNAppKeyString;

static FNAppKeyString appkey_buf;

/*
  appkeys are variable length strings. The protocol allows for
  variable length packets but there's currently no way to find out how
  long the reply data is. We'll expect a reply with an unsigned 16 bit
  integer header to describe the actual length.
*/

bool fuji_bus_appkey_read(void *string, uint16_t *length)
{
  // Caller may not have room for length header so use our own buffer to read
  if (!FUJICALL_RV(FUJICMD_READ_APPKEY, &appkey_buf, sizeof(appkey_buf)))
    return false;
  *length = appkey_buf.length;
  memmove(string, appkey_buf.data, appkey_buf.length);
  return true;
}

bool fuji_bus_appkey_write(void *string, uint16_t length)
{
  return FUJICALL_D(FUJICMD_WRITE_APPKEY, string, length);
}
