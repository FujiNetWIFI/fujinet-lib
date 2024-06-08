#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_close_directory()
{
	uint8_t pl[1];
	pl[0] = FUJICMD_CLOSE_DIRECTORY;
	return fuji_open_close(1, pl);
}
