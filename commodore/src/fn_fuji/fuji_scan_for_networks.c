#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_scan_for_networks(uint8_t *count)
{
	int bytes_read;
	return open_read_close(FUJICMD_SCAN_NETWORKS, true, &bytes_read, 1, count);
}
