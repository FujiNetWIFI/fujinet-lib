#include <stdint.h>
#include <string.h>

#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

void fn_fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
{
	int8_t err = 0;
	err = sp_get_fuji_id();
	if (err <= 0) {
		return;
	}

	// Number of bytes in payload
	sp_payload[0] = 0;
	sp_payload[1] = 2;
	// Actual payload
	sp_payload[2] = src_slot;
	sp_payload[3] = dst_slot;
	strcpy(&sp_payload[2], copy_spec);

	sp_error = sp_control(sp_fuji_id, 0xD8);
}
