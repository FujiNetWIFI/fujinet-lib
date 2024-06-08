#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_wifi_status(uint8_t *status)
{
	uint8_t pl[1];
	pl[0] = FUJICMD_GET_WIFISTATUS;
	return open_read_close(1, pl, 1, status);
}
