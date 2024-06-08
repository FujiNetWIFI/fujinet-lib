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
	return open_close(3, pl);
}
