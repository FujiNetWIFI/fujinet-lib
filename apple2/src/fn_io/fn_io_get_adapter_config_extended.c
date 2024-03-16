#include <stdint.h>
#include <string.h>

#include "fujinet-io.h"
#include "fujinet-network-apple2.h"

// do status call to FN with code 0xC4
void fn_io_get_adapter_config_extended(AdapterConfigExtended *acx)
{
	int8_t err = 0;
	err = sp_find_fuji();
	if (err <= 0) {
		return;
	}

	err = sp_status(sp_fuji_id, 0xC4);
	if (err != 0) {
		return;
	}

	memcpy(acx, &sp_payload[0], sizeof(AdapterConfigExtended));
}
