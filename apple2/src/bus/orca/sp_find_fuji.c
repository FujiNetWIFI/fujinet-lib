#include <stdint.h>
#include <stdbool.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

uint8_t sp_fuji_id = 0;

bool sp_find_fuji() {
    int r = sp_find_device("THE_FUJI");
	if (r <= 0) {
		// backwards compatible hack check
		r = sp_find_device("FUJINET_DISK_0");
		if (r <= 0) {
			sp_fuji_id = 0;
			return false;
		}
	}
	sp_fuji_id = (uint8_t) (r & 0xFF);
	return true;
}

uint8_t sp_get_fuji_id()
{
	if (sp_fuji_id != 0) {
		return sp_fuji_id;
	}

	sp_find_fuji();
	return sp_fuji_id;
}