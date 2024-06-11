#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
{
	uint8_t *pl;
	uint16_t pl_len;
	bool ret;

	pl_len = strlen(buffer) + 3 + 1; // add 1 for the null string terminator, although technically not required as we go by lengths

	pl = malloc(pl_len);
	if (pl == NULL) {
		status_error();
		return false;
	}

	pl[0] = ds;
	pl[1] = hs;
	pl[2] = mode;
	strcpy((char *) &pl[3], buffer);
	pl[pl_len - 1] = '\0';

	ret = open_close_data(FUJICMD_SET_DEVICE_FULLPATH, pl_len, pl);
	free(pl);
	return ret;
}
