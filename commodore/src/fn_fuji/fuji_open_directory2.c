#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_open_directory2(uint8_t hs, char *path, char *filter)
{
	uint8_t path_len;
	uint8_t filter_len = 0;
	uint16_t data_size;
	uint8_t *data;
	bool is_success;
	int bytes_read;

	path_len = strlen(path);
	if (filter != NULL) {
		filter_len = strlen(filter);
	}

	data_size = path_len + filter_len + 3;  // 2 for string nul chars, 1 for hs
	data = malloc(data_size);
	if (data == NULL) {
		status_error(ERROR_MALLOC_FAILED, FUJICMD_OPEN_DIRECTORY);
		return false;
	}

	memset(data, 0, data_size);
	data[0] = hs;
	memcpy(&data[1], path, path_len);
	if (filter != NULL) {
		memcpy(&data[2 + path_len], filter, filter_len);
	}

	is_success = open_close_data(FUJICMD_OPEN_DIRECTORY, true, data_size, data);
	free(data);
	return is_success;
}
