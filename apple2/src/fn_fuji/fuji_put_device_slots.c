#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_put_device_slots(DeviceSlot *d, size_t size)
{
	uint16_t payload_size = sizeof(DeviceSlot) * size;
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_payload[0] = payload_size & 0xFF;
	sp_payload[1] = (payload_size & 0xFF00) >> 8;

	memcpy(&sp_payload[2], d, payload_size);
	sp_error = sp_control(sp_fuji_id, FUJICMD_WRITE_DEVICE_SLOTS);
	return sp_error == 0;
}
