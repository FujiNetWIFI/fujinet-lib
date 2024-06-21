#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_hash_input(char *s, uint16_t len)
{
	return open_close_data(FUJICMD_HASH_INPUT, true, len, (uint8_t *) s);
}
