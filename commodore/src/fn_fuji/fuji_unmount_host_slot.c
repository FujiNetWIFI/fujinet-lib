#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"

bool fuji_unmount_host_slot(uint8_t hs)
{
	uint8_t pl[2];
	pl[0] = 0xF9;
	pl[1] = hs;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 2, (uint8_t *) pl) != 0) {
		return false;
	}

	cbm_close(FUJI_CMD_CHANNEL);
	return true;
}
