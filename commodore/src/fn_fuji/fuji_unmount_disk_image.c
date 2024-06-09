#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_unmount_disk_image(uint8_t ds)
{
	uint8_t pl[2];
	pl[0] = FUJICMD_UNMOUNT_IMAGE;
	pl[1] = ds;
	return open_close(2, pl);
}
