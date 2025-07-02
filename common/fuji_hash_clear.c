#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "bus.h"

bool fuji_hash_clear()
{
	return open_close(FUJICMD_HASH_CLEAR);
}
