#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_set_boot_mode(uint8_t mode)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = 1;
	sp_payload[1] = 0;
	sp_payload[2] = mode;

	sp_error = sp_control(sp_fuji_id, FUJICMD_SET_BOOT_MODE);
	return sp_error == 0;

}
