#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_unmount_disk_image(uint8_t ds)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_payload[0] = 1;
	sp_payload[1] = 0;
	sp_payload[2] = ds;

	sp_error = sp_control(sp_fuji_id, FUJICMD_UNMOUNT_IMAGE);
	return sp_error == 0;

}
