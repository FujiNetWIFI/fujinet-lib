#include "fujinet-bus-coleco.h"
#include "fujinet-appkey.h"
#include "fujinet-commands.h"
#include "fujinet-const.h"

#include <string.h>

/*
  appkeys are variable length strings. The cartridge brokers to the ESP32 over
  the RS232 FujiBus path, and rs232Fuji::appkey_read() prepends a little-endian
  uint16_t length to the key data before sending it -- so the reply carries a
  header even though the bus itself has variable length packets. Read it into
  our own buffer, because the caller only promised room for the key.
*/

typedef struct {
  uint16_t length;
  uint8_t data[MAX_APPKEY_LEN];
} FNAppKeyString;

static FNAppKeyString appkey_buf;

bool fuji_bus_appkey_read(void *string, uint16_t *length)
{
  if (!FUJICALL_RV(FUJICMD_READ_APPKEY, &appkey_buf, sizeof(appkey_buf)))
    return false;

  if (appkey_buf.length > MAX_APPKEY_LEN)
    return false;

  *length = appkey_buf.length;
  memmove(string, appkey_buf.data, appkey_buf.length);

  return true;
}

bool fuji_bus_appkey_write(const void *string, uint16_t length)
{
  return FUJICALL_D(FUJICMD_WRITE_APPKEY, string, length);
}
