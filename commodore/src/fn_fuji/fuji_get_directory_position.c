#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_directory_position(uint16_t *pos)
{
	int bytes_read;
	return open_read_close(FUJICMD_GET_DIRECTORY_POSITION, true, &bytes_read, 2, (uint8_t *) pos);
}
