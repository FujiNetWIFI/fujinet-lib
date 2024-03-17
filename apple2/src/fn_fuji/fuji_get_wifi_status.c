#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

#define NO_SSID_AVAILABLE 1

uint8_t fuji_get_wifi_status(void)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return NO_SSID_AVAILABLE;
	}

	// IWM only currently returns either 3 (WL_CONNECTED), or 6 (WL_DISCONNECTED), but there are other codes.
	// If that changes in Firmware, nothing to do here, it'll automatically return new values.
	sp_error = sp_status(sp_fuji_id, 0xFA);
	if (sp_error == 0) {
		return sp_payload[0];
	}

	return NO_SSID_AVAILABLE;
}
