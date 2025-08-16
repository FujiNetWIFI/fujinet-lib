#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-network-adam.h"

extern enum AppKeySize fn_adam_keysize;

void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize)
{
    fn_adam_creator_id = creator_id;
    fn_adam_app_id = app_id;
    fn_adam_keysize = keysize;
}
