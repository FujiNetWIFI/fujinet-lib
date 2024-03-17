#include <stdint.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

bool fn_io_reset(void)
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
