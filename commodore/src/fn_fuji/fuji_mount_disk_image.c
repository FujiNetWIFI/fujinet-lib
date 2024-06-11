#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
{
	return open_close_data_2(FUJICMD_MOUNT_IMAGE, ds, mode);
}
