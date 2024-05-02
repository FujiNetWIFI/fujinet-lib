#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"
#include "sp.h"

bool fuji_get_host_slots(HostSlot *h, size_t size)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, 0xF4);
	if (sp_error == 0) {
		if (sp_count != sizeof(HostSlot) * size) {
			// didn't receive the correct amount of data for array we are filling
			sp_error = SP_ERR_IO_ERROR;
			return false;
		}
		memcpy(h, &sp_payload[0], sp_count);
	}
	return sp_error == 0;

}
