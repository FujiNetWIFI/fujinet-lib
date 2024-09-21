#include <stdint.h>
#include <string.h>

#include "fujinet-clock.h"
#include "fujinet-bus-apple2.h"

uint8_t clock_set_tz(char *tz) {
	uint16_t len;
	int8_t result;

	sp_get_clock_id();
	if (sp_clock_id == 0) return FN_ERR_IO_ERROR;

	len = strlen(tz) + 1;

	sp_payload[0] = len & 0xFF;
	sp_payload[1] = len >> 8;
	memcpy(&sp_payload[2], tz, len);

	result = sp_control(sp_clock_id, 'T');
	return fn_error(result);
}