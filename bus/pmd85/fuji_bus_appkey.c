#include <fujinet-const.h>
#include <fujinet-int.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>
#include <fujinet-appkey.h>
#include <string.h>

typedef struct {
  uint16_t length;
  uint8_t data[MAX_APPKEY_LEN];
} FNAppKeyString;

static FNAppKeyString appkey_buf;

/*
  appkeys are variable length strings but drivewireFuji in the FujiNet
  firmware uses fixed length packets, same as on the CoCo. On write
  the aux1/aux2 fields are combined into a uint16_t. On read, there is
  an extra 2 bytes of header to indicate how much of the fixed block
  represents the string.
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

bool fuji_bus_appkey_write(const void *string, uint16_t length)
{
  return FUJICALL_B12_D(FUJICMD_WRITE_APPKEY, length, string, MAX_APPKEY_LEN);
}
