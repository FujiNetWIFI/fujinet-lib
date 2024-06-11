#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_boot_config(uint8_t toggle)
{
	return open_close_data_1(FUJICMD_CONFIG_BOOT, toggle);
}
