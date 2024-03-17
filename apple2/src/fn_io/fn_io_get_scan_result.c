#include <stdint.h>
#include <string.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

void fn_io_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return;
	}

	sp_payload[0] = 1;
	sp_payload[1] = 0;
	sp_payload[2] = n;
	sp_error = sp_control(sp_fuji_id, 0xFC);
	if (sp_error != 0) {
		return;
	}

	sp_error = sp_status(sp_fuji_id, 0xFC);
    if (sp_error == 0) {
        memcpy(ssid_info, &sp_payload[0], sizeof(SSIDInfo));
    }

}
