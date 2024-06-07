#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-atari.h"
#include "fujinet-fuji-atari.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;

uint8_t t_fuji_read_appkey[7] = { FUJICMD_READ_APPKEY, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };

bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
{
	uint8_t open_data[6];
	uint8_t mode = 0;				// READ with standard 64 byte size
	uint16_t buffer_size = 64;		// 64 is default, gets adjusted if mode is different
	uint8_t *buffer;  				// the malloc'd buffer to read into before copying to user's data buffer
	uint16_t bytes_read;

	if (ak_creator_id == 0) {
		return false;
	}

	// WIP
	// if (ak_appkey_size == SIZE_256) {
	// 	mode = 3;
	// 	buffer_size = 256;
	// }

	buffer = malloc(buffer_size + 2); // 2 for length bytes
	if (!buffer) return false;

	open_data[0] = ak_creator_id & 0xFF;
	open_data[1] = (ak_creator_id >> 8) & 0xFF;
	open_data[2] = ak_app_id;
	open_data[3] = key_id;
	open_data[4] = mode;
	open_data[5] = 0; 		// reserved byte

	copy_fuji_cmd_data(&t_fuji_open_appkey[0]);
	OS.dcb.dbuf = open_data;
	bus();
	if (!fuji_success()) {
		free(buffer);
		return false;
	}

	copy_fuji_cmd_data(&t_fuji_read_appkey[0]);
	OS.dcb.dbuf = buffer;
	OS.dcb.dbyt = buffer_size + 2; // add 2 for count bytes
	bus();
	if (!fuji_success()) {
		free(buffer);
		return false;
	}

	bytes_read = buffer[0] + (buffer[1] << 8);
	*count = bytes_read;
	if (bytes_read > 0) {
		// skip the first 2 bytes, copy buffer data to user's data
		memcpy(data, &buffer[2], bytes_read);
	}
	free(buffer);
	return true;

}