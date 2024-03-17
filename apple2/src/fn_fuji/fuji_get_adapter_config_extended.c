#include <stdint.h>
#include <string.h>

#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

// do status call to FN with code 0xC4
void fuji_get_adapter_config_extended(AdapterConfigExtended *acx)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return;
	}

	sp_error = sp_status(sp_fuji_id, 0xC4);
	if (sp_error == 0) {
		memcpy(acx, &sp_payload[0], sizeof(AdapterConfigExtended));
	}

}
