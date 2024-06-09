#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_disable_device(uint8_t d)
{
	uint8_t pl[2];
	pl[0] = FUJICMD_DISABLE_DEVICE;
	pl[1] = d;
	return open_close(2, pl);
}
