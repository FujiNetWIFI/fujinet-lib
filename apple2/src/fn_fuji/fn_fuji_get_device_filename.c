#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

// send (ds + $a0) as status code, as the status code to read a device goes from A0 to A7.
void fn_fuji_get_device_filename(uint8_t ds, char *buffer)
{
	uint8_t stat = ds + 0xA0;

	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return;
	}

	sp_error = sp_status(sp_fuji_id, stat);
	if (sp_error == 0) {
		memcpy(buffer, &sp_payload[0], 256);
	}
}
