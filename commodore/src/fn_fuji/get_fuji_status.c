#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

// This one gets called a lot, so we save some instructions by preparing its command data
uint8_t status_cmd[2] = { 0x01, 0x53 };

// allow for up to 41 char message (including the null terminator), plus 3 bytes for error/connected/channel
uint8_t status_data[44];

bool get_fuji_status()
{
	int bytes_read;

	// ensure there are no old strings in the buffer
	memset(status_data, 0, sizeof(status_data));

	// do a status call to find out if anything went wrong
	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 2, status_cmd) != 0) {
		status_error();
		return false;
	}

	bytes_read = cbm_read(FUJI_CMD_CHANNEL, status_data, 80);
	cbm_close(FUJI_CMD_CHANNEL);

	iec_error = status_data[0];
	iec_connected = status_data[1] != 0; // 0 is false, anything else is true
	iec_channel = status_data[2]; // this may always simply be 15
	iec_message = (char *) &status_data[3]; // message is always null terminated, so can be treated as a string.

	return true;
}