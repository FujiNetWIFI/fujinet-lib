#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

// Similar to A2, is this really a thing needed? If so, needs fixing in FN firmware
bool fuji_get_device_enabled_status(uint8_t d)
{
	return true;
}
