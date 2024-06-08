#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
	uint8_t pl[3];
	pl[0] = FUJICMD_GET_SCAN_RESULT;
	pl[1] = n;
	pl[2] = 0x00;
	return open_read_close(3, pl, sizeof(SSIDInfo), (uint8_t *) ssid_info);
}
