#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
{
	uint8_t *pl;
	uint8_t pl_len;
	bool ret;
	size_t copy_spec_len = strlen(copy_spec);

	// with the header bytes, we can only send 251 bytes of path data
	if (copy_spec_len > 251) {
		return false;
	}
	pl_len = copy_spec_len + 3 + 1; // add 1 for the null string terminator, although technically not required as we go by lengths

	pl = malloc(pl_len);
	if (pl == NULL) {
		return false;
	}

	pl[0] = FUJICMD_COPY_FILE;
	pl[1] = src_slot;
	pl[2] = dst_slot;
	strcpy((char *) &pl[3], copy_spec);
	pl[pl_len - 1] = '\0';

	ret = open_close(pl_len, pl);
	free(pl);

	return ret;

}
