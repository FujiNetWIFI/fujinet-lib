#include <fujinet-appkey.h>
#include <fujinet-fuji.h>

typedef struct {
  uint16_t creator;
  uint8_t app_id;
  uint8_t key_id;
  uint8_t mode;
  uint8_t reserved;
} FNAppKeyID;

uint16_t ak_creator_id;
uint8_t ak_app_id;
enum AppKeySize ak_appkey_size;
static FNAppKeyID appkey;

static void init_appkey(uint8_t key_id, uint8_t mode)
{
  uint8_t idx = 0;


  appkey.creator = ak_creator_id;
  appkey.app_id = ak_app_id;
  appkey.key_id = key_id;
  appkey.mode = mode;
  appkey.reserved = 0;
  return;
}

bool fuji_read_appkey(uint8_t key_id, uint16_t *length, uint8_t *data)
{
  init_appkey(key_id, 0);
  if (!FUJICALL_D(FUJICMD_OPEN_APPKEY, &appkey, sizeof(appkey)))
    return false;
  return fuji_bus_appkey_read(data, length);
}

bool fuji_write_appkey(uint8_t key_id, uint16_t length, uint8_t *data)
{
  init_appkey(key_id, 1);
  if (!FUJICALL_D(FUJICMD_OPEN_APPKEY, &appkey, sizeof(appkey)))
    return false;
  return fuji_bus_appkey_write(data, length);
}
