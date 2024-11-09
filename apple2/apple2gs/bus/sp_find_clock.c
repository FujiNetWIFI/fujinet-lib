#include <stdint.h>
#include <stdbool.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

uint8_t sp_clock_id = 0;

bool sp_find_clock(void) {
    int r = sp_find_device("FN_CLOCK");
	if (r <= 0) {
		sp_clock_id = 0;
		return false;
	}
	sp_clock_id = (uint8_t) (r & 0xFF);
	return true;
}

uint8_t sp_get_clock_id(void)
{
	if (sp_clock_id != 0) {
		return sp_clock_id;
	}

	sp_find_clock();
	return sp_clock_id;
}