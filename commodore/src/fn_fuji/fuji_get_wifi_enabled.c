#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_wifi_enabled()
{
	uint8_t pl[1];
	uint8_t is_enabled_value = 0;
	bool ret;

	pl[0] = FUJICMD_GET_WIFI_ENABLED;
	ret = open_read_close(1, pl, 1, &is_enabled_value);
	if (!ret) {
		// there was a problem, we couldn't get status, so return false
		return false;
	}
	// convert the value from 0 as false, otherwise true
	return is_enabled_value != 0;
}
