#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_close_directory(void)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_error = sp_control(sp_fuji_id, 0xF5);
	return sp_error == 0;
}
