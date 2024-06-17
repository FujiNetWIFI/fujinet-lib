#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern uint8_t __oserror;

// All the things, it has data to write with the command, and reads a value back, and a status
bool open_read_close_data(uint8_t cmd, bool should_close, int *bytes_read, uint16_t params_size, uint8_t *cmd_params, uint16_t result_size, uint8_t *result_data)
{
	int bytes_written;
	bool is_success;

	if (!open_or_write(cmd)) {
		return false;
	}

	is_open = true;

	bytes_written = cbm_write(FUJI_CMD_CHANNEL, cmd_params, params_size);
	if (bytes_written != params_size) {
		*bytes_read = 0;
	}

	if (bytes_written == params_size) {
		// only do the read if the command data was sent successfully
		*bytes_read = cbm_read(FUJI_CMD_CHANNEL, result_data, result_size);
	}

	is_success = get_fuji_status(should_close);

	if (bytes_written != params_size) {
		// Failure sending command, e.g. wrong parameters sent. status string etc will be in _fuji_status.
		// Force a close if it wouldn't have happened in the status. We definitely want to close, but if "should_close" was false we need to manually do it here
		if (!should_close) cbm_close(FUJI_CMD_CHANNEL);
		is_open = false;
		return false;
	}
	return is_success;
}

bool open_read_close_data_1(uint8_t cmd, int *bytes_read, uint8_t param1, uint16_t result_size, uint8_t *result_data)
{
	uint8_t pl[1];
	pl[0] = param1;
	return open_read_close_data(cmd, true, bytes_read, 1, pl, result_size, result_data);
}

bool open_read_close_data_2(uint8_t cmd, int *bytes_read, uint8_t param1, uint8_t param2, uint16_t result_size, uint8_t *result_data)
{
	uint8_t pl[2];
	pl[0] = param1;
	pl[1] = param2;
	return open_read_close_data(cmd, true, bytes_read, 2, pl, result_size, result_data);
}
