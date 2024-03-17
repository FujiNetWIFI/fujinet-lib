#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_reset(void)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = 0x00;
	sp_payload[1] = 0x00;

	sp_error = sp_control(sp_fuji_id, 0xFF);
	return sp_error == 0;

}
