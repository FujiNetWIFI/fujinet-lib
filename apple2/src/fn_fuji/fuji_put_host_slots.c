#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_put_host_slots(HostSlot *h, size_t size)
{
	uint16_t payload_size = sizeof(HostSlot) * size;
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_payload[0] = payload_size & 0xFF;
	sp_payload[1] = (payload_size & 0xFF00) >> 8;

	memcpy(&sp_payload[2], h, payload_size);
	sp_error = sp_control(sp_fuji_id, FUJICMD_WRITE_HOST_SLOTS);
	return sp_error == 0;

}
