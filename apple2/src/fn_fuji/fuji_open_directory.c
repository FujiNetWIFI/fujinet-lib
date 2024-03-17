#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_open_directory(uint8_t hs, char *path_filter)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	// Number of bytes in payload
	// we don't care about the length of the path/filter, it's max is 255 (256 - 1 for hs)
	// The FN will split the string on null bytes anyway, so no need to worry about
	// it on the host side too much.
	sp_payload[0] = 0;
	sp_payload[1] = 1;
	// Actual payload
	sp_payload[2] = hs;
	memcpy(&sp_payload[3], path_filter, 255);

	sp_error = sp_control(sp_fuji_id, 0xf7);
	return sp_error == 0;

}
