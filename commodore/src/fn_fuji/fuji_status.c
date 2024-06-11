#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_status(FNStatus *status)
{
	int bytes_read;
	return open_read_close(FUJICMD_STATUS, &bytes_read, sizeof(FNStatus), (uint8_t *) status);
}
