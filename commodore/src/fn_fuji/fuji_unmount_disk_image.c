#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_unmount_disk_image(uint8_t ds)
{
	return open_close_data_1(FUJICMD_UNMOUNT_IMAGE, ds);
}
