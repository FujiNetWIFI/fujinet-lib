#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	if (ak_creator_id == 0) {
		return false;
	}

	sp_payload[0] = 6;
	sp_payload[1] = 0;
	sp_payload[2] = ak_creator_id & 0xFF;
	sp_payload[3] = (ak_creator_id >> 8) & 0xFF;
	sp_payload[4] = ak_app_id;
	sp_payload[5] = key_id;
	sp_payload[6] = 0x01; 			// WRITE mode
	sp_payload[7] = 0x00;			// reserved

	sp_error = sp_control(sp_fuji_id, 0xDC);
	if (sp_error != 0) {
		return false;
	}

	sp_payload[0] = count & 0xFF;
	sp_payload[1] = (count >> 8) & 0xFF;

	memcpy(&sp_payload[2], data, count);

	sp_error = sp_control(sp_fuji_id, 0xDE);
	return sp_error == 0;
}
