#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
{
	size_t filename_len = strlen(buffer);
	if (filename_len >= 254) {
		return false;
	}

    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = (filename_len + 4) & 0xFF; // 3 params, plus file length + 1 for nul
	sp_payload[1] = 0;
	sp_payload[2] = ds;
	sp_payload[3] = hs;
	sp_payload[4] = mode;
	strcpy((char *) &sp_payload[5], buffer);

	sp_error = sp_control(sp_fuji_id, 0xE2);
	return sp_error == 0;

}
