#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#else
#include <stdint.h>
#endif /* CMOC_VERSION_ */

#include "fujinet-fuji.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;

void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize)
{
	ak_appkey_size = keysize;
	ak_app_id = app_id;
	ak_creator_id = creator_id;
}
