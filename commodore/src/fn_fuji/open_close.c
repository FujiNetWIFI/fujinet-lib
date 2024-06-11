#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern uint8_t cmd_args[2];

// no data to send in this version, it's just the command for immediate actions, no return data. we do query the status though.
bool open_close(uint8_t cmd)
{
	cmd_args[1] = cmd;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 2, cmd_args) != 0) {
		status_error();
		return false;
	}

	cbm_close(FUJI_CMD_CHANNEL);
	return get_fuji_status();
}