#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

// Stupidly complex, the FN ALWAYS returns "1" for IWM, and ignores the device id.
// However, if it is implemented on FN, this will not need changing.
bool fuji_get_device_enabled_status(uint8_t d)
{
	int8_t err = 0;
	err = sp_get_fuji_id();
	if (err <= 0) {
		return false;
	}

	// Number of bytes in payload
	sp_payload[0] = 1;
	sp_payload[1] = 0;

	// Actual payload
	sp_payload[2] = d;

	// Call FN
	sp_error = sp_control(sp_fuji_id, 0xD1);
	if (sp_error == 0) {
		return sp_payload[0] != 0;
	}
	return false;
}
