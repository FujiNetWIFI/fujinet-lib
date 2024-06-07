#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_get_directory_position(uint16_t *pos)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, FUJICMD_GET_DIRECTORY_POSITION);
    if (sp_error == 0) {
        *pos = sp_payload[0] + (sp_payload[1] << 8);
    }

    return sp_error == 0;
}
