#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-atari.h"
#include "fujinet-fuji-atari.h"

uint8_t t_fuji_write_appkey[7] = { 0xde, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00 };

bool fuji_write_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint16_t count, uint8_t *data)
{
	uint8_t open_data[6];
	uint8_t mode = 0;				// READ with standard 64 byte size

	open_data[0] = creator_id & 0xFF;
	open_data[1] = (creator_id >> 8) & 0xFF;
	open_data[2] = app_id;
	open_data[3] = key_id;
	open_data[4] = 1;		// WRITE mode
	open_data[5] = 0; 		// reserved byte

	copy_fuji_cmd_data(_t_fuji_open_appkey);
	OS.dcb.dbuf = open_data;
	bus();
	if (!fuji_success()) return false;

	copy_fuji_cmd_data(t_fuji_write_appkey);
	OS.dcb.dbuf = data;
	OS.dcb.dbyt = count;
	bus();
	return fuji_success();
}