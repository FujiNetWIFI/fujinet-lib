#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "bus.h"

bool fuji_set_status()
{
	return open_close(FUJICMD_SET_STATUS);
}
