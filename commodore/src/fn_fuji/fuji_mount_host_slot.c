#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_mount_host_slot(uint8_t hs)
{
	return open_close_data_1(FUJICMD_MOUNT_HOST, hs);
}
