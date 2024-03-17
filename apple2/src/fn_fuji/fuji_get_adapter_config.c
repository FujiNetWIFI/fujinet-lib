#include <stdint.h>
#include <string.h>

#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

// do status call to FN with code 0xe8, payload[0] = 0
bool fuji_get_adapter_config(AdapterConfig *ac)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, 0xE8);
	if (sp_error == 0) {
		memcpy(ac, &sp_payload[0], sizeof(AdapterConfig));
	}
	return sp_error == 0;
}
