#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_enable_device(uint8_t d)
{
	uint8_t pl[2];
	pl[0] = FUJICMD_ENABLE_DEVICE;
	pl[1] = d;
	return open_close(2, pl);
}
