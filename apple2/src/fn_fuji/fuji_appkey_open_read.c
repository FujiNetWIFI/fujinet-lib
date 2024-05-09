#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_read_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint16_t *count, uint8_t *data, enum AppKeySize keysize)
{
	uint8_t mode = 0;
	uint16_t buffer_size = 64;
	
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	if (keysize == SIZE_256) {
		mode = 3;
		buffer_size = 256;
	}

	sp_payload[0] = 6;
	sp_payload[1] = 0;
	sp_payload[2] = creator_id & 0xFF;
	sp_payload[3] = (creator_id >> 8) & 0xFF;
	sp_payload[4] = app_id;
	sp_payload[5] = key_id;
	sp_payload[6] = mode;
	sp_payload[7] = 0;			// reserved
	
	sp_error = sp_control(sp_fuji_id, 0xDC);
	if (sp_error != 0) return false;

	sp_error = sp_status(sp_fuji_id, 0xDD);
	if (sp_error == 0) {
		memset(data, 0, buffer_size);
		*count = sp_count;
		if (sp_count > 0 && sp_count <= buffer_size) {
			memcpy(data, &sp_payload[0], sp_count);
		}
	}
	return sp_error == 0;
}
