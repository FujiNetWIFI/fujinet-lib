#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_boot_config(uint8_t toggle)
{
	uint8_t pl[2];
	pl[0] = FUJICMD_CONFIG_BOOT;
	pl[1] = toggle;
	return open_close(2, pl);
}
