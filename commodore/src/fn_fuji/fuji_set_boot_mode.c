#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_boot_mode(uint8_t mode)
{
	uint8_t pl[2];
	pl[0] = FUJICMD_SET_BOOT_MODE;
	pl[1] = mode;
	return open_close(2, pl);
}
