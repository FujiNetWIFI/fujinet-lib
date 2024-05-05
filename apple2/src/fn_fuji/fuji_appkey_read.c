#include <stdint.h>
#include <string.h>
#include "../../../fujinet-fuji.h"
#include "../../../fujinet-bus-apple2.h"

uint8_t fuji_appkey_read(AppKeyRead *buffer)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

    sp_error = sp_status(sp_fuji_id, 0xDD);
	if (sp_error == 0) {
        buffer->length = 0;
        if (sp_count>0 && sp_count <= MAX_APPKEY_LEN) {
            memcpy(buffer->value, &sp_payload[0], sp_count);
            buffer->length = sp_count;
        }
	}
	return sp_error == 0;
}
