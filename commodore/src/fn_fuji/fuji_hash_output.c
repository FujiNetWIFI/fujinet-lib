#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)
{
	int bytes_read;
	return open_read_close_data_1(FUJICMD_HASH_OUTPUT, &bytes_read, output_type, len, (uint8_t *) s);
}
