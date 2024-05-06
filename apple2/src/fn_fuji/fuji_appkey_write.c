#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"
#include <string.h>

// writes app-key, returns error status
bool fuji_appkey_write(uint16_t count, AppKeyWrite *buffer)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = 64;
	sp_payload[1] = 0;

	// Sticking with a 64 payload size. Can we use smaller?
	memset(&sp_payload[2], 0, 64);

	if (count>MAX_APPKEY_LEN)
		count = MAX_APPKEY_LEN;
			
	memcpy(&sp_payload[2], buffer, count);

	sp_error = sp_control(sp_fuji_id, 0xDE);
	return sp_error == 0;
}
