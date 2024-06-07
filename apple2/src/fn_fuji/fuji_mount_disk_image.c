#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_payload[0] = 2;
	sp_payload[1] = 0;
	sp_payload[2] = ds;
	sp_payload[3] = mode;

	sp_error = sp_control(sp_fuji_id, FUJICMD_MOUNT_IMAGE);
	return sp_error == 0;
}
