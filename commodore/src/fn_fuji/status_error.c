#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

// in case we are unable to get the status, this will be the error message
char *status_error_message = "status error";

// indicate we were perform comms with FN in the last command
void status_error()
{
	iec_error = 1;	// force an error value
	iec_connected = false;
	iec_channel = 0;
	iec_message = status_error_message;
}