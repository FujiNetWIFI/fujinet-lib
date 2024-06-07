#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
{
	uint8_t pl[3];
	pl[0] = FUJICMD_MOUNT_IMAGE;
	pl[1] = ds;
	pl[2] = mode;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 3, (uint8_t *) pl) != 0) {
		return false;
	}

	cbm_close(FUJI_CMD_CHANNEL);
	return true;
}
