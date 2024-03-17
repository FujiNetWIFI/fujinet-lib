#include <stdint.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

bool fn_io_mount_host_slot(uint8_t hs)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_error = sp_control(sp_fuji_id, 0xF9);
	return sp_error == 0;
}
