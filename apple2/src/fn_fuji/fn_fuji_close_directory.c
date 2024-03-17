#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

void fn_fuji_close_directory(void)
{
	int8_t err = 0;
	err = sp_get_fuji_id();
	if (err <= 0) {
		return;
	}

	sp_control(sp_fuji_id, 0xF5);
}
