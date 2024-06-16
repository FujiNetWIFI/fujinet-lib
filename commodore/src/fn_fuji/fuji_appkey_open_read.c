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
	uint16_t buffer_size = 64; // TODO: key sizes overhaul
	int bytes_read;
	bool is_success;
	uint8_t pl[6];

	if (ak_creator_id == 0) {
		return false;
	}

	pl[0] = ak_creator_id & 0xFF;
	pl[1] = ak_creator_id >> 8;
	pl[2] = ak_app_id;
	pl[3] = key_id;
	pl[4] = mode;
	pl[5] = 0;

	// send the creator / app / mode values
	if (!open_close_data(FUJICMD_OPEN_APPKEY, false, 6, pl)) {
		*count = 0;
		return false;
	}

	// creator data sent fine, read the key
	is_success = open_read_close(FUJICMD_READ_APPKEY, true, &bytes_read, buffer_size, data);
	// a bit shitty, cbm_read returns an int, so +/- 32k, we want just uint16_t, so check for negative.
	// keys are really small anyway, so not an issue on >32k, but negative values are errors we have catered for in the _fuji_status values
	*count = (bytes_read >= 0) ? bytes_read : 0;
	return is_success;
}
