#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_get_wifi_enabled(void)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, FUJICMD_GET_WIFI_ENABLED);
	if (sp_error == 0) {
		return sp_payload[0] != 0;
	}
	return false;
}
