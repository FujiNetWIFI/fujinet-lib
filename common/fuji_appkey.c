#include "fujinet-fuji.h"
#include <string.h>

#define APPKEY_BLOCK_SIZE 64 // FIXME - use ak_appkey_size somehow

static uint16_t ak_creator_id;
static uint8_t ak_app_id;
static enum AppKeySize ak_appkey_size;

typedef struct {
  uint16_t creator;
  uint8_t app_id;
  uint8_t key_id;
  uint8_t mode;
  uint8_t reserved;
} FNAppKeyID;

typedef struct {
  uint16_t block_size;
  uint16_t length;
  uint8_t data[APPKEY_BLOCK_SIZE];
} FNAppKeyData;

static FNAppKeyID appkey;
static FNAppKeyData appkey_buf;

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

bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
{
  FNAppKeyData *ak_data = (FNAppKeyData *) data;


  init_appkey(key_id, 0);
  if (!FUJICALL_D(FUJICMD_OPEN_APPKEY, &appkey, sizeof(appkey)))
    return false;

  if (!FUJICALL_RV(FUJICMD_READ_APPKEY, ak_data, sizeof(FNAppKeyData)))
    return false;
  *count = ak_data->length;
  memmove(data, ak_data->data, sizeof(ak_data->data));
  return true;
}

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
  FNAppKeyData *ak_data = (FNAppKeyData *) data;


  init_appkey(key_id, 1);
  if (!FUJICALL_D(FUJICMD_OPEN_APPKEY, &appkey, sizeof(appkey)))
    return false;
  appkey_buf.length = count;
  memcpy(appkey_buf.data, data, APPKEY_BLOCK_SIZE);
  return FUJICALL_D(FUJICMD_WRITE_APPKEY, &appkey_buf.length,
                    sizeof(appkey_buf) - sizeof(appkey_buf.block_size));
}

void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize)
{
  ak_appkey_size = keysize;
  ak_app_id = app_id;
  ak_creator_id = creator_id;
}
