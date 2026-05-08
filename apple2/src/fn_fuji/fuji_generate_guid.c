#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_generate_guid(char *buffer)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, FUJICMD_GENERATE_GUID);
	if (sp_error == 0) {
		memcpy(buffer, &sp_payload[0], MAX_GUID_LEN);
	}
	return sp_error == 0;
}