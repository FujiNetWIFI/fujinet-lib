#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_mount_all()
{
	uint8_t pl[1];
	pl[0] = FUJICMD_MOUNT_ALL;
	return open_close(1, pl);
}
