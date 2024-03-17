#include <stdint.h>
#include <string.h>

#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

// do status call to FN with code 0xe8, payload[0] = 0
void fn_io_get_adapter_config(AdapterConfig *ac)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return;
	}

	sp_error = sp_status(sp_fuji_id, 0xE8);
	if (sp_error == 0) {
		memcpy(ac, &sp_payload[0], sizeof(AdapterConfig));
	}

}
