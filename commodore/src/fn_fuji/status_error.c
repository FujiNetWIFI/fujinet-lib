#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

// 19 bytes, if you change this, change the define
char *status_error_message = "status error";
#define STATUS_ERR_MESSAGE_LENGTH 12

// indicate we were perform comms with FN in the last command
void status_error()
{
	_fuji_status.value.error = 1;
	_fuji_status.value.channel = 0;
	_fuji_status.value.connected = 0;
	strcpy(&_fuji_status.value.msg[0], status_error_message);
	_fuji_status.value.msg[STATUS_ERR_MESSAGE_LENGTH] = '\0'; 
}