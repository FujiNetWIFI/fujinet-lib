#include <stdint.h>
#include <string.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

// Copies 8 host slots (32 char strings) into given buffer
void fn_io_get_host_slots(HostSlot *h)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return;
	}

	sp_error = sp_status(sp_fuji_id, 0xF4);
    if (sp_error == 0) {
        memcpy(h, &sp_payload[0], 256); // 32 * 8
    }
}
