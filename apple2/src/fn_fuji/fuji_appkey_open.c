#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

uint8_t fuji_appkey_open(AppKeyOpen *buffer)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = 5; // Packet size
	sp_payload[1] = 0;
	sp_payload[2] = buffer->creator & 0xFF;
	sp_payload[3] = (buffer->creator & 0xFF00) >> 8;
	sp_payload[4] = buffer->app;
	sp_payload[5] = buffer->key;
	sp_payload[6] = buffer->mode;
	
	sp_error = sp_control(sp_fuji_id, 0xDC);
	return sp_error == 0;
}
