#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
	int bytes_read;
	return open_read_close_data_1(FUJICMD_GET_SCAN_RESULT, &bytes_read, n, sizeof(SSIDInfo), (uint8_t *) ssid_info);
}
