#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

// Use 0x, and NOT string literals to avoid CC65 charmap translations. These are bytes not chars.
uint8_t fuji_status_cmd[2] = { 0x01, 0x53 };

// An internal version of fuji_status called at the end of the open_ commands
// which uses the currently open connection to write the status command to.
// This function also closes the connection if requested to, which allows multi stage commands to do them all on single open to reduce transfers
bool get_fuji_status(bool should_close)
{
	int bytes_read;
	int bytes_written;

	// ensure there are no old strings in the buffer
	memset(&_fuji_status.raw[0], 0, sizeof(FNStatus));

	// do a status call to find out if anything went wrong. Using the current open channel, so just write our bytes
	bytes_written = cbm_write(CBM_CMD_CHANNEL, fuji_status_cmd, 2);
	if (bytes_written != 2) {
		// always close on an error
		cbm_close(CBM_CMD_CHANNEL);
		status_error(ERROR_STATUS_FAILED_TO_WRITE, 0x53);
		return false;
	}

	bytes_read = cbm_read(CBM_CMD_CHANNEL, &_fuji_status.raw[0], sizeof(FNStatus));
	if (should_close) {
		cbm_close(CBM_CMD_CHANNEL);
		fuji_is_open = false;
	}

	// return true if the error is 0 (i.e. no error)
	return _fuji_status.value.error == 0;

}