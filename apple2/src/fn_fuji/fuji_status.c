#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_status(FNStatus *status)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, 0x53);
	if (sp_error == 0) {
		memcpy(status, &sp_payload[0], sizeof(FNStatus));
	}
	return sp_error == 0;

}
