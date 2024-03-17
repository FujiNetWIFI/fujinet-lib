#include <stdint.h>
#include <string.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

bool fn_io_put_device_slots(DeviceSlot *d)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	// payload size is 304 = 0x0130 (8 * sizeof(DeviceSlot))
	sp_payload[0] = 0x30;
	sp_payload[1] = 0x01;

	memcpy(&sp_payload[2], d, 0x0130);
	sp_error = sp_control(sp_fuji_id, 0xF1);
	return sp_error == 0;
}
