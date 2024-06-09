#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
{
	uint8_t *pl;
	uint8_t pl_len;
	bool ret;
	size_t filename_len = strlen(buffer);

	// with the header bytes, we can only send 250 bytes of path data
	if (filename_len > 250) {
		return false;
	}
	pl_len = filename_len + 4 + 1; // add 1 for the null string terminator, although technically not required as we go by lengths

	pl = malloc(pl_len);
	if (pl == NULL) {
		return false;
	}

	pl[0] = FUJICMD_SET_DEVICE_FULLPATH;
	pl[1] = ds;
	pl[2] = hs;
	pl[3] = mode;
	strcpy((char *) &pl[4], buffer);
	pl[pl_len - 1] = '\0';

	ret = open_close(pl_len, pl);
	free(pl);

	return ret;
}
