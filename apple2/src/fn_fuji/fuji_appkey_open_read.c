#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;

bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
{
	uint8_t mode = 0;
	uint16_t buffer_size = 64;

	if (sp_get_fuji_id() == 0) {
		return false;
	}

	if (ak_creator_id == 0) {
		return false;
	}

	// WIP
	// if (ak_appkey_size == SIZE_256) {
	// 	mode = 3;
	// 	buffer_size = 256;
	// }

	sp_payload[0] = 6;
	sp_payload[1] = 0;
	sp_payload[2] = ak_creator_id & 0xFF;
	sp_payload[3] = (ak_creator_id >> 8) & 0xFF;
	sp_payload[4] = ak_app_id;
	sp_payload[5] = key_id;
	sp_payload[6] = mode;
	sp_payload[7] = 0;			// reserved
	
	sp_error = sp_control(sp_fuji_id, FUJICMD_OPEN_APPKEY);
	if (sp_error != 0) return false;

	sp_error = sp_status(sp_fuji_id, FUJICMD_READ_APPKEY);
	if (sp_error == 0) {
		memset(data, 0, buffer_size);
		*count = sp_count;
		if (sp_count > 0 && sp_count <= buffer_size) {
			memcpy(data, &sp_payload[0], sp_count);
		}
	}
	return sp_error == 0;
}
