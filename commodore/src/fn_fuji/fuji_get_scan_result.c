#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
	int bytes_read;
	uint8_t pl[3];
	pl[0] = 0xFC;
	pl[1] = n;
	pl[2] = 0x00;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 3, (uint8_t *) pl) != 0) {
		return false;
	}

	bytes_read = cbm_read(FUJI_CMD_CHANNEL, ssid_info, sizeof(SSIDInfo));
	cbm_close(FUJI_CMD_CHANNEL);
	return bytes_read > 0;
}
