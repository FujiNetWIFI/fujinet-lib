#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_directory_position(uint16_t *pos)
{
	uint8_t pl[1];
	pl[0] = FUJICMD_GET_DIRECTORY_POSITION;
	return open_read_close(1, pl, 2, (uint8_t *) pos);
}
