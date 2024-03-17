#include <stdint.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

uint16_t fn_io_get_directory_position()
{
    uint16_t pos = 0;
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return 0;
	}

	sp_error = sp_status(sp_fuji_id, 0xE5);
    if (sp_error == 0) {
        pos = sp_payload[0] + (sp_payload[1] << 8);
    }

    return pos;
}
