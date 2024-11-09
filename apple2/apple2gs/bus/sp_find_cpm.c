#include <stdint.h>
#include <stdbool.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

uint8_t sp_cpm_id = 0;

bool sp_find_cpm(void) {
    int r = sp_find_device("CPM");
	if (r <= 0) {
		sp_cpm_id = 0;
		return false;
	}
	sp_cpm_id = (uint8_t) (r & 0xFF);
	return true;
}

uint8_t sp_get_cpm_id(void)
{
	if (sp_cpm_id != 0) {
		return sp_cpm_id;
	}

	sp_find_cpm();
	return sp_cpm_id;
}