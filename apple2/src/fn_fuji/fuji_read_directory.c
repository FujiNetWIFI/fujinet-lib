#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

char *fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return NULL;
	}

	sp_payload[0] = 2;
	sp_payload[1] = 0;
	sp_payload[2] = maxlen;
	sp_payload[3] = aux2;

	sp_error = sp_control(sp_fuji_id, 0xF6);
	buffer[0] = 0;
	if (sp_error != 0) {
		return buffer;
	}

	sp_error = sp_status(sp_fuji_id, 0xF6);
    if (sp_error == 0) {
        strcpy(buffer, sp_payload);
    }
	return buffer;

}
