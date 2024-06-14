#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

// returns the status of running the command to fetch the status, it does not return the value of the status, that's in the field passed to us.
bool fuji_status(FNStatus *status)
{
	int bytes_read;
	bool is_success;
	memset(status, 0, sizeof(FNStatus));
	is_success = open_read_close(FUJICMD_STATUS, &bytes_read, sizeof(FNStatus), (uint8_t *) status);
	if (!is_success) {
		status_error();
	}
	return is_success;
}
