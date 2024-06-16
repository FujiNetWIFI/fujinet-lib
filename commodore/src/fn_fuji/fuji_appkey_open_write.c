#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
	bool ret;
	uint8_t pl[6];
	uint8_t *out_data;

	if (ak_creator_id == 0) {
		return false;
	}

	out_data = malloc(count);
	if (out_data == NULL) {
		status_error(ERROR_MALLOC_FAILED, FUJICMD_WRITE_APPKEY);
		return false;		
	}
	memset(out_data, 0, count);

	pl[0] = ak_creator_id & 0xFF;
	pl[1] = ak_creator_id >> 8;
	pl[2] = ak_app_id;
	pl[3] = key_id;
	pl[4] = 0x01; 				// WRITE mode
	pl[5] = 0;					// reserved

	// send the creator / app / mode values
	if (!open_close_data(FUJICMD_OPEN_APPKEY, false, 6, pl)) {
		printf("ERROR! not sending appkey data\n");
		return false;
	}

	// now do a write of the key data, on IEC we don't need to send the count, as the write doesn't have to be a fixed size
	memcpy(out_data, data, count);

	ret = open_close_data(FUJICMD_WRITE_APPKEY, true, count, out_data);
	free(out_data);
	return ret;
}
