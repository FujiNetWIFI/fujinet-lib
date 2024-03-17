#include <stdint.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

bool fn_io_mount_disk_image(uint8_t ds, uint8_t mode)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_error = sp_control(sp_fuji_id, 0xF8);
	return sp_error == 0;
}
