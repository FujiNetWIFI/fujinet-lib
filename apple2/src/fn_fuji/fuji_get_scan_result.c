#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = 1;
	sp_payload[1] = 0;
	sp_payload[2] = n;
	sp_error = sp_control(sp_fuji_id, FUJICMD_GET_SCAN_RESULT);
	if (sp_error != 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, FUJICMD_GET_SCAN_RESULT);
    if (sp_error == 0) {
        memcpy(ssid_info, &sp_payload[0], sizeof(SSIDInfo));
    }
	return sp_error == 0;

}
