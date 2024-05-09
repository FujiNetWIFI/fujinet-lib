#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_write_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint16_t count, uint8_t *data)
{
	sp_error = sp_get_fuji_id();
	if (sp_error != 0) {
		return false;
	}

	sp_payload[0] = 6;
	sp_payload[1] = 0;
	sp_payload[2] = creator_id & 0xFF;
	sp_payload[3] = (creator_id >> 8) & 0xFF;
	sp_payload[4] = app_id;
	sp_payload[5] = key_id;
	sp_payload[6] = 1; 			// WRITE mode
	sp_payload[7] = 0;			// reserved
	
	sp_error = sp_control(sp_fuji_id, 0xDC);
	if (sp_error != 0) {
		return false;
	}

	sp_payload[0] = count & 0xFF;
	sp_payload[1] = (count << 8) & 0xFF;

	memcpy(&sp_payload[2], data, count);

	sp_error = sp_control(sp_fuji_id, 0xDE);
	return sp_error == 0;
}
