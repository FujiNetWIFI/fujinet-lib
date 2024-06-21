#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_hash_compute_no_clear(uint8_t type)
{
	return open_close_data_1(FUJICMD_HASH_COMPUTE_NO_CLEAR, type);
}
