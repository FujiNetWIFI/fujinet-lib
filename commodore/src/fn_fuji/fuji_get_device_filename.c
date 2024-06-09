#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_device_filename(uint8_t ds, char *buffer)
{
	uint8_t pl[2];
	uint8_t *filename;
	bool is_success;

	filename = malloc(256);
	if (filename == NULL) {
		return false;
	}
	memset(filename, 0, 256);

	pl[0] = FUJICMD_GET_DEVICE_FULLPATH;
	pl[1] = ds;

	is_success = open_read_close(2, pl, 256, (uint8_t *) filename);
	if (is_success) {
		strcpy(buffer, filename);
	}
	free(filename);
	return is_success;
}
