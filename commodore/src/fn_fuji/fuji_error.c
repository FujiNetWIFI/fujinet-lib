#include <stdbool.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_error()
{
	return iec_error != 0;
}
