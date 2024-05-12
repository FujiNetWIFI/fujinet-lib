#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_mount_host_slot(uint8_t hs)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_payload[0] = 1;
	sp_payload[1] = 0;
	sp_payload[2] = hs;

	sp_error = sp_control(sp_fuji_id, 0xF9);
	return sp_error == 0;
}
