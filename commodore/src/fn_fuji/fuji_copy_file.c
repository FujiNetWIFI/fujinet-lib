#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
{
	uint8_t *pl;
	uint16_t pl_len;
	bool ret;

	pl_len = strlen(copy_spec) + 2 + 1; // add 1 for the null string terminator, although technically not required as we go by lengths

	pl = malloc(pl_len);
	if (pl == NULL) {
		return false;
	}

	pl[0] = src_slot;
	pl[1] = dst_slot;
	strcpy((char *) &pl[2], copy_spec);
	pl[pl_len - 1] = '\0';

	ret = open_close_data(FUJICMD_COPY_FILE, true, pl_len, pl);
	free(pl);

	return ret;
}
