#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_device_filename(uint8_t ds, char *buffer)
{
	uint8_t *filename;
	bool is_success;
	int bytes_read;

	filename = malloc(256);
	if (filename == NULL) {
		return false;
	}
	memset(filename, 0, 256);

	is_success = open_read_close_data_1(FUJICMD_GET_DEVICE_FULLPATH, &bytes_read, ds, 256, (uint8_t *) filename);
	if (is_success) {
		strcpy(buffer, filename);
	}
	free(filename);
	return is_success;
}
