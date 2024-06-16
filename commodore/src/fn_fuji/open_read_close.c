#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern char cmd_args[2];

bool open_or_write(uint8_t cmd)
{
	int bytes_written;
	uint8_t err_code = 0;
	if (is_open) {
		// this is a continuation, so use the existing channel and write the data instead of 
		bytes_written = cbm_write(FUJI_CMD_CHANNEL, cmd_args, 2);
		if (bytes_written != 2) {
			err_code = ERROR_OPEN_WRITE_CMD_FAILED;
		}
	} else {
		if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 2, cmd_args) != 0) {
			err_code = ERROR_OPEN_CMD_FAILED;
		}
	}
	if (err_code != 0) {
		status_error(err_code, cmd);
		cbm_close(FUJI_CMD_CHANNEL);
		is_open = false;
		return false;
	}
	return true;
}

// no parameters in this version, we just fetch results after it's executed
bool open_read_close(uint8_t cmd, bool should_close, int *bytes_read, uint16_t result_size, uint8_t *result_data)
{
	int bytes_written;
	cmd_args[1] = cmd;

	if (!open_or_write(cmd)) {
		*bytes_read = 0;
		return false;
	}

	*bytes_read = cbm_read(FUJI_CMD_CHANNEL, result_data, result_size);
	return get_fuji_status(should_close);
}