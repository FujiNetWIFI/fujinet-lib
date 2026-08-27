#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_qrcode_output(char *s, uint16_t len)
{
	uint8_t params[2];
	int bytes_read;

	params[0] = len & 0xFF;
	params[1] = len >> 8;

	return open_read_close_data(FUJICMD_QRCODE_OUTPUT, true, &bytes_read,
		sizeof(params), params, len, (uint8_t *) s);
}
