#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_scan_for_networks(uint8_t *count)
{
	int bytes_read;
	uint8_t pl[1];
	pl[0] = FUJICMD_SCAN_NETWORKS;
	return open_read_close(1, pl, 1, count);
}
