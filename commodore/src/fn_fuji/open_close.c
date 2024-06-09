#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool open_close(uint8_t size, uint8_t *data)
{
	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, size, data) != 0) {
		return false;
	}

	cbm_close(FUJI_CMD_CHANNEL);
	return true;
}