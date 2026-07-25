#include <fujinet-fuji.h>

typedef struct {
  uint16_t creator;
  uint8_t app_id;
  uint8_t key_id;
  uint8_t mode;
  uint8_t reserved;
} FNAppKeyID;

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;
extern FNAppKeyID appkey;
