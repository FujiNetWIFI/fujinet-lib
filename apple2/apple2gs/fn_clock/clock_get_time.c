#include <stdint.h>
#include <string.h>

#include "fujinet-clock.h"
#include "fujinet-bus-apple2.h"

// these correspond to the enum TimeFormat
const char format_cmds[] = { 'T', 'P', 'A', 'B', 'S', 'Z' };

uint8_t clock_get_time(uint8_t* tz, TimeFormat format) {
	uint8_t result;

	sp_get_clock_id();

	if (sp_clock_id == 0) return FN_ERR_IO_ERROR;
	if (format >= 6) return FN_ERR_BAD_CMD;

	result = sp_status(sp_clock_id, format_cmds[format]);	
	if (result != 0) return FN_ERR_IO_ERROR;

	memcpy(tz, sp_payload, sp_count);
	return FN_ERR_OK;
}