#include <stdbool.h>
#include <stdint.h>

#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_reset()
{
	uint8_t pl[1];
	pl[0] = 0xFF;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 1, (uint8_t *) pl) != 0) {
		return false;
	}
	cbm_close(FUJI_CMD_CHANNEL);
	return true;
}
