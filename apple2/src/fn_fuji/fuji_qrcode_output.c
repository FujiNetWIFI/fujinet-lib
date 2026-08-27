#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_qrcode_output(char *s, uint16_t len)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = 2;
	sp_payload[1] = 0;
	sp_payload[2] = len & 0xFF;
	sp_payload[3] = (len & 0xFF00) >> 8;

	// This command is destructive on the FujiNet side: the bytes it sends are
	// erased from its buffer. The CONTROL must therefore run exactly once, and
	// the STATUS below only drains the already queued reply.
	sp_error = sp_control(sp_fuji_id, FUJICMD_QRCODE_OUTPUT);
	if (sp_error != 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, FUJICMD_QRCODE_OUTPUT);
	if (sp_error == 0) {
		memcpy(s, &sp_payload[0], len);
	}
	return sp_error == 0;
}
