#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_wifi_enabled()
{
	int bytes_read;
	uint8_t is_enabled_value = 0;

	if (!open_read_close(FUJICMD_GET_WIFI_ENABLED, &bytes_read, 1, &is_enabled_value)) {
		return false;
	}
	// convert the value from 0 as false, otherwise true
	return is_enabled_value != 0;
}
