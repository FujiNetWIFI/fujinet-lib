#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

// no data to send in this version, it's just the command for immediate actions, no return data. we do query the status though.
bool open_close(uint8_t cmd)
{
	if (!open_or_write(cmd)) {
		// the current status is set for us as there was a critical error, so we just return false, and allow client to check the status.
		return false;
	}

	// Data transfers etc were fine, but was the result of operation a success?
	return get_fuji_status(true);
}