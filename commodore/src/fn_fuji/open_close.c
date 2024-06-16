#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern uint8_t cmd_args[2];

// no data to send in this version, it's just the command for immediate actions, no return data. we do query the status though.
bool open_close(uint8_t cmd)
{
	cmd_args[1] = cmd;
	if (!open_or_write(cmd)) {
		return false;
	}

	return get_fuji_status(true);
}