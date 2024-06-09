#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;

bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
{
	uint8_t mode = 0;
	uint16_t buffer_size = 64;
	int bytes_read;
	uint8_t pl[7];

	if (ak_creator_id == 0) {
		return false;
	}

	pl[0] = FUJICMD_OPEN_APPKEY;
	pl[1] = ak_creator_id & 0xFF;
	pl[2] = ak_creator_id >> 8;
	pl[3] = ak_app_id;
	pl[4] = key_id;
	pl[5] = mode;
	pl[6] = 0;

	// send the creator / app / mode values
	if (!open_close(7, pl)) {
		// something went wrong, abort
		return false;
	}

	// now do a read of the key
	pl[0] = FUJICMD_READ_APPKEY;
	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 1, (uint8_t *) pl) != 0) {
		return false;
	}

	bytes_read = cbm_read(FUJI_CMD_CHANNEL, data, buffer_size);
	if (bytes_read <= 0) {
		*count = 0;
	} else {
		*count = bytes_read;
	}
	cbm_close(FUJI_CMD_CHANNEL);
	return bytes_read > 0;
}
