#include <stdbool.h>
#include <stdint.h>

#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_reset()
{
	return open_close(FUJICMD_RESET);
}
