#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_mount_all()
{
	return open_close(FUJICMD_MOUNT_ALL);
}
