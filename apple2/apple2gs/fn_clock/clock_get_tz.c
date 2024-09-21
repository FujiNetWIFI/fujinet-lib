#include <stdint.h>
#include <string.h>

#include "fujinet-clock.h"
#include "fujinet-bus-apple2.h"

uint8_t clock_get_tz(uint8_t *tz) {
	uint8_t result;

	sp_get_clock_id();
	if (sp_clock_id == 0) return FN_ERR_IO_ERROR;

	result = sp_status(sp_clock_id, 'G');
	if (result == 0) return FN_ERR_IO_ERROR;

	memcpy(tz, sp_payload, sp_count);
	return FN_ERR_OK;
}