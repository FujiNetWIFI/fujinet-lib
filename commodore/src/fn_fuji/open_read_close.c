#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool open_read_close(uint8_t out_size, uint8_t *out, uint16_t in_size, uint8_t *in)
{
	int bytes_read;
	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, out_size, out) != 0) {
		return false;
	}

	bytes_read = cbm_read(FUJI_CMD_CHANNEL, in, in_size);
	cbm_close(FUJI_CMD_CHANNEL);
	return bytes_read > 0;
}