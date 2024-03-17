#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

// This actually points to an array of device slots, not a single, and all 8 are copied back
void fn_fuji_get_device_slots(DeviceSlot *d)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return;
	}

	sp_error = sp_status(sp_fuji_id, 0xF2);
	if (sp_error == 0) {
		memcpy(d, &sp_payload[0], sp_count);
	}

}
