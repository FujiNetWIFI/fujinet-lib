#include <stdint.h>
#include <string.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

bool fn_io_put_host_slots(HostSlot *h)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	// payload size is 256 = 0x0100 (8 * sizeof(HostSlot))
	sp_payload[0] = 0x00;
	sp_payload[1] = 0x01;

	memcpy(&sp_payload[2], h, 0x0100);
	sp_error = sp_control(sp_fuji_id, 0xF3);
	return sp_error == 0;

}
