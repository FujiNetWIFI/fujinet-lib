#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

// send (ds + $a0) as status code, as the status code to read a device goes from A0 to A7.
// ASSUMPTION: The buffer can hold up to a 256 byte string
bool fuji_get_device_filename(uint8_t ds, char *buffer)
{
	uint8_t stat = ds + 0xA0;

	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, stat);
	if (sp_error == 0) {
		memcpy(buffer, &sp_payload[0], 256);
	}
	return sp_error == 0;
}
