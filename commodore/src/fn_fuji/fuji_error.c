#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern uint8_t __oserror;

bool fuji_error()
{
	// probably needs some work, we have __oserror, internally we aren't keeping track of anything at the moment
	return __oserror;
}
