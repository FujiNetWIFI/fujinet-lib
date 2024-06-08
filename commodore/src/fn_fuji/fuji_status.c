#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_status(FNStatus *status)
{
	uint8_t pl[1];
	pl[0] = FUJICMD_STATUS;
	return open_read_close(1, pl, sizeof(FNStatus), (uint8_t *) status);
}
