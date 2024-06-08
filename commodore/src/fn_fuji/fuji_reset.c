#include <stdbool.h>
#include <stdint.h>

#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_reset()
{
	uint8_t pl[1];
	pl[0] = 0xFF;
	return open_close(1, pl);
}
