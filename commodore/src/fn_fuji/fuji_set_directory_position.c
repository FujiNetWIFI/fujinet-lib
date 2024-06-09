#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_directory_position(uint16_t pos)
{
	uint8_t pl[3];
	pl[0] = FUJICMD_SET_DIRECTORY_POSITION;
	pl[1] = pos & 0xFF;
	pl[2] = pos >> 8;
	return open_close(3, pl);
}
