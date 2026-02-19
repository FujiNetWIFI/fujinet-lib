#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-atari.h"
#include "fujinet-fuji-atari.h"


uint8_t t_fuji_generate_guid[6] = { FUJICMD_GENERATE_GUID, 0x40, 0x00, 0x00, 0x00, 0x00 };

bool fuji_generate_guid(char *buffer)
{
    uint8_t data[MAX_GUID_LEN + 2]; // add 2 for count bytes
	uint16_t buffer_size = MAX_GUID_LEN;
	uint16_t bytes_read;

	copy_fuji_cmd_data(t_fuji_generate_guid);
	OS.dcb.dbuf   = data;
	OS.dcb.dbyt   = buffer_size + 2; 	// add 2 for count bytes
	OS.dcb.dtimlo = 1;					// Generating GUID should be nearly instantaneous, make timeout 1 second
	bus();
	if (!fuji_success()) {
		return false;
	}

	bytes_read = data[0] + (data[1] << 8);
	if (bytes_read > 0) {
		// move the data from data+2 to data, and set everything else to 0 after this. 
		memmove(data, &data[2], bytes_read);
        strncpy(buffer, (char*)data, buffer_size);
	}
	return true;

}