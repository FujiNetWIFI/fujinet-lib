#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_qrcode_input(char *s, uint16_t len)
{
	uint16_t payload_size = len + 2;

	if (sp_get_fuji_id() == 0) {
		return false;
	}

	// sp_payload[0..1] is the total decoded length, covering parameters and
	// data together; the FujiNet pops parameters off the front of it.
	sp_payload[0] = payload_size & 0xFF;
	sp_payload[1] = (payload_size & 0xFF00) >> 8;

	sp_payload[2] = len & 0xFF;
	sp_payload[3] = (len & 0xFF00) >> 8;
	memcpy(&sp_payload[4], s, len);

	sp_error = sp_control(sp_fuji_id, FUJICMD_QRCODE_INPUT);
	return sp_error == 0;
}
