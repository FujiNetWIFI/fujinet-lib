#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_close_directory(void)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_payload[0] = 0x00;
	sp_payload[1] = 0x00;

	sp_error = sp_control(sp_fuji_id, FUJICMD_CLOSE_DIRECTORY);
	return sp_error == 0;
}
