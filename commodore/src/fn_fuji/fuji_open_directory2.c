#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_open_directory2(uint8_t hs, char *path, char *filter)
{
	uint8_t err;
	uint8_t path_len;
	uint8_t filter_len = 0;
	uint16_t data_size;
	uint8_t *data;

	path_len = strlen(path);
	if (filter != NULL) {
		filter_len = strlen(filter);
	}
	data_size = path_len + filter_len + 4;  // 1 for cmd byte, 1 for hostslot, 2 for string nul chars
	if (data_size > 255) {
		// can't be too long, the max data we can specify in the name is 255.
		// TODO: can we turn this into sending 2 strings of arbitrary length after the open? i.e. write data after open.
		return false;
	}

	data = malloc(data_size);
	memset(data, 0, data_size);

	data[0] = FUJICMD_OPEN_DIRECTORY;
	data[1] = hs;
	memcpy(&data[2], path, path_len);
	if (filter != NULL) {
		memcpy(&data[3 + path_len], filter, filter_len);
	}

	err = fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, data_size, data);
	free(data);
	if (err == 0) {
		cbm_close(FUJI_CMD_CHANNEL);
		return true;
	}
	return false;

}
