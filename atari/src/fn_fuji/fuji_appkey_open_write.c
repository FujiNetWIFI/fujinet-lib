#include <atari.h>
#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-atari.h"
#include "fujinet-fuji-atari.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;

uint8_t t_fuji_write_appkey[7] = { FUJICMD_WRITE_APPKEY, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00 };

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
	uint8_t open_data[6];
	uint8_t mode = 0;				// READ with standard 64 byte size
	uint16_t keysize = 64;

	if (ak_creator_id == 0) {
		return false;
	}

	// WIP
	// if (ak_appkey_size == SIZE_256) {
	// 	keysize = 256;
	// }

	// this is horrid in cc65's output!
	open_data[0] = ak_creator_id & 0xFF;
	open_data[1] = (ak_creator_id >> 8) & 0xFF;
	open_data[2] = ak_app_id;
	open_data[3] = key_id;
	open_data[4] = 1;		// WRITE mode
	open_data[5] = 0; 		// reserved byte

	copy_fuji_cmd_data((uint8_t *) &t_fuji_open_appkey[0]);
	OS.dcb.dbuf = open_data;
	bus();
	if (!fuji_success()) return false;

	copy_fuji_cmd_data(&t_fuji_write_appkey[0]);
	OS.dcb.dbuf = data;
	OS.dcb.dbyt = keysize;		// we have to specify the 64/256/... value in dbyt
	OS.dcb.daux = count;		// ... but the count can be less in daux to tell the FN how much of the buffer needs to be written
	bus();
	return fuji_success();
}