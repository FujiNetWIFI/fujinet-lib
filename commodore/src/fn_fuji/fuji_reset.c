#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"

bool fuji_reset()
{
	cbm_open(15, FUJI_CBM_DEV, 15, "\xFF");
	cbm_close(15);
	return true;
}
