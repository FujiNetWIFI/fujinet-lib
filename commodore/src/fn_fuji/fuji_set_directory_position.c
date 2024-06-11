#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_directory_position(uint16_t pos)
{
	return open_close_data_2(FUJICMD_SET_DIRECTORY_POSITION, pos & 0xFF, pos >> 8);
}
