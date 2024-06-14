#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

uint8_t status_cmd[2] = { 0x01, 0x53 };

// Fetch the status, and act on its value directly
bool get_fuji_status()
{
	int bytes_read;

	// ensure there are no old strings in the buffer
	memset(&_fuji_status.raw[0], 0, sizeof(FNStatus));

	// do a status call to find out if anything went wrong
	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 2, status_cmd) != 0) {
		status_error();
		return false;
	}

	bytes_read = cbm_read(FUJI_CMD_CHANNEL, &_fuji_status.raw[0], sizeof(FNStatus));
	cbm_close(FUJI_CMD_CHANNEL);

	// return true if the error is 0 (i.e. no error)
	return _fuji_status.value.error == 0;

}