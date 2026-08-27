#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_qrcode_input(char *s, uint16_t len)
{
	uint8_t *pl;
	bool ret;

	// The IEC bus takes parameters and payload as one stream, so the two byte
	// length parameter has to be prepended to the data itself.
	pl = malloc(len + 2);
	if (pl == NULL) {
		return false;
	}

	pl[0] = len & 0xFF;
	pl[1] = len >> 8;
	memcpy(&pl[2], s, len);

	ret = open_close_data(FUJICMD_QRCODE_INPUT, true, len + 2, pl);
	free(pl);

	return ret;
}
