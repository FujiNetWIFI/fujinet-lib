#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#else
#include <stdint.h>
#endif /* CMOC_VERSION_ */

#include "fujinet-fuji.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;

// IMPORTANT: Instead of "enum AppKeySize", explicitly matching the underlying uint8_t data type, othwerise
// CMOC will write a 16 bit enum value at the location, overflowing into the next byte, causing corruption
extern uint8_t ak_appkey_size; 

void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize)
{
	ak_appkey_size = (uint8_t)keysize;
	ak_app_id = app_id;
	ak_creator_id = creator_id;
}
