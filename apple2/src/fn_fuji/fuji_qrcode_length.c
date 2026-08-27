#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = 1;
	sp_payload[1] = 0;
	sp_payload[2] = output_mode;

	// CONTROL runs the handler; the reply it queues is drained by the STATUS
	// that follows, without the handler running a second time.
	sp_error = sp_control(sp_fuji_id, FUJICMD_QRCODE_LENGTH);
	if (sp_error != 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, FUJICMD_QRCODE_LENGTH);
	if (sp_error == 0) {
		memcpy(len, &sp_payload[0], sizeof(unsigned long));
	}
	return sp_error == 0;
}
