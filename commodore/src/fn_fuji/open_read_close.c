#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern char cmd_args[2];

// no parameters in this version, we just fetch results after it's executed
bool open_read_close(uint8_t cmd, int *bytes_read, uint16_t result_size, uint8_t *result_data)
{
	cmd_args[1] = cmd;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 2, cmd_args) != 0) {
		*bytes_read = 0;
		status_error();
		return false;
	}

	// get result from the command
	*bytes_read = cbm_read(FUJI_CMD_CHANNEL, result_data, result_size);
	cbm_close(FUJI_CMD_CHANNEL);

	// finally return the status of the command, if we're not the status command ourselves
	if (cmd != FUJICMD_STATUS) {
		return get_fuji_status();
	} else {
		return true;
	}
}