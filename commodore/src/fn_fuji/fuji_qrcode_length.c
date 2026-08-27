#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len)
{
	int bytes_read;

	return open_read_close_data_1(FUJICMD_QRCODE_LENGTH, &bytes_read, output_mode,
		sizeof(unsigned long), (uint8_t *) len);
}
