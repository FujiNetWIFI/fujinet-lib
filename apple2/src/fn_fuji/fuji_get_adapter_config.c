#include <stdint.h>
#include <string.h>

#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_get_adapter_config(AdapterConfig *ac)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, 0xE8);
	if (sp_error == 0) {
		memcpy(ac, &sp_payload[0], sizeof(AdapterConfig));
	}
	return sp_error == 0;
}
