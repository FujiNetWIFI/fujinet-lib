#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_unmount_host_slot(uint8_t hs)
{
	uint8_t pl[2];
	pl[0] = FUJICMD_MOUNT_HOST;
	pl[1] = hs;
	return open_close(2, pl);
}
