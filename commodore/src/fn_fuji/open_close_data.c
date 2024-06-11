#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

// All commands are <01><cmd>, we reuse this buffer to send all data, and only have to set 2nd byte
uint8_t cmd_args[2] = { 0x01, 0x00 };

// this is a command that has no return data, so just push command, its data, and read the status
bool open_close_data(uint8_t cmd, uint16_t params_size, uint8_t *cmd_params)
{
	cmd_args[1] = cmd;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 2, cmd_args) != 0) {
		status_error();
		return false;
	}

	// write the cmd parameters
	cbm_write(FUJI_CMD_CHANNEL, cmd_params, params_size);
	cbm_close(FUJI_CMD_CHANNEL);

	// finally return the status of the command
	return get_fuji_status();
}

bool open_close_data_1(uint8_t cmd, uint8_t param1)
{
	uint8_t pl[1];
	pl[0] = param1;
	return open_close_data(cmd, 1, pl);
}

bool open_close_data_2(uint8_t cmd, uint8_t param1, uint8_t param2)
{
	uint8_t pl[2];
	pl[0] = param1;
	pl[1] = param2;
	return open_close_data(cmd, 2, pl);
}
