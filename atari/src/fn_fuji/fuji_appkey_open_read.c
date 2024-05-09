#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-atari.h"
#include "fujinet-fuji-atari.h"

uint8_t t_fuji_read_appkey[7] = { 0xdd, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };

bool fuji_read_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint16_t *count, uint8_t *data, enum AppKeySize keysize)
{
	uint8_t open_data[6];
	uint8_t mode = 0;				// READ with standard 64 byte size
	uint16_t buffer_size = 64;		// 64 is default
	uint8_t *buffer;  				// the malloc'd buffer to read into before copying to user's data buffer
	uint16_t bytes_read;

	if (keysize == SIZE_256) {
		mode = 3;
		buffer_size = 256;
	}

	buffer = malloc(buffer_size + 2); // 2 for length bytes
	if (!buffer) return false;

	open_data[0] = creator_id & 0xFF;
	open_data[1] = (creator_id >> 8) & 0xFF;
	open_data[2] = app_id;
	open_data[3] = key_id;
	open_data[4] = mode;
	open_data[5] = 0; 		// reserved byte

	copy_fuji_cmd_data(_t_fuji_open_appkey);
	OS.dcb.dbuf = open_data;
	bus();
	if (!fuji_success()) {
		free(buffer);
		return false;
	}

	copy_fuji_cmd_data(t_fuji_read_appkey);
	OS.dcb.dbuf = data;
	OS.dcb.dbyt = buffer_size;
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