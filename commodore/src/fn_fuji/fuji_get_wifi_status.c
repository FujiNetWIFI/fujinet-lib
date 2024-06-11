#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_wifi_status(uint8_t *status)
{
	int bytes_read;
	return open_read_close(FUJICMD_GET_WIFISTATUS, &bytes_read, 1, status);
}
