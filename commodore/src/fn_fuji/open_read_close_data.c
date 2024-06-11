#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern char cmd_args[2];

// All the things, it has data to write with the command, and reads a value back, and a status
bool open_read_close_data(uint8_t cmd, int *bytes_read, uint16_t params_size, uint8_t *cmd_params, uint16_t result_size, uint8_t *result_data)
{
	cmd_args[1] = cmd;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 2, cmd_args) != 0) {
		*bytes_read = 0;
		status_error();
		return false;
	}

	// write the cmd parameters
	cbm_write(FUJI_CMD_CHANNEL, cmd_params, params_size);
	cbm_close(FUJI_CMD_CHANNEL);

	// get result from the command
	*bytes_read = cbm_read(FUJI_CMD_CHANNEL, result_data, result_size);
	cbm_close(FUJI_CMD_CHANNEL);

	// finally return the status of the command
	return get_fuji_status();
}

bool open_read_close_data_1(uint8_t cmd, int *bytes_read, uint8_t param1, uint16_t result_size, uint8_t *result_data)
{
	uint8_t pl[1];
	pl[0] = param1;
	return open_read_close_data(cmd, bytes_read, 1, pl, result_size, result_data);
}

bool open_read_close_data_2(uint8_t cmd, int *bytes_read, uint8_t param1, uint8_t param2, uint16_t result_size, uint8_t *result_data)
{
	uint8_t pl[2];
	pl[0] = param1;
	pl[1] = param2;
	return open_read_close_data(cmd, bytes_read, 2, pl, result_size, result_data);
}
