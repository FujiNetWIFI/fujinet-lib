#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_set_directory_position(uint16_t pos)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = 2;
	sp_payload[1] = 0;
	sp_payload[2] = pos & 0xFF;
	sp_payload[3] = (pos >> 8) & 0xFF;

	sp_error = sp_control(sp_fuji_id, 0xE4);
	return sp_error == 0;

}
