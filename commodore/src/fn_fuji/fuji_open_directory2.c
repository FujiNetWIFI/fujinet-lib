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

	data_size = path_len + filter_len + 2;  // 2 for string nul chars
	data = malloc(data_size);
	if (data == NULL) {
		status_error();
		return false;
	}

	memset(data, 0, data_size);
	memcpy(data, path, path_len);
	if (filter != NULL) {
		memcpy(&data[1 + path_len], filter, filter_len);
	}

	is_success = open_read_close_data_1(FUJICMD_OPEN_DIRECTORY, &bytes_read, hs, data_size, data);
	free(data);
	return is_success;
}
