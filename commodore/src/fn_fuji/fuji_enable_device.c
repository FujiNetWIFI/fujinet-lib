#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_enable_device(uint8_t d)
{
	return open_close_data_1(FUJICMD_ENABLE_DEVICE, d);
}
