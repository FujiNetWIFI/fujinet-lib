#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_ssid(NetConfig *nc)
{
	bool is_success;
	uint8_t ssid_len;
	uint8_t pass_len;
	uint8_t data_size;
	uint8_t *data;

	ssid_len = strlen(nc->ssid);
	pass_len = strlen(nc->password);
	data_size = ssid_len + pass_len + 2;  // 2 for string nul chars

	data = malloc(data_size);
	memset(data, 0, data_size);

	memcpy(&data[0], nc->ssid, ssid_len);
	memcpy(&data[1 + ssid_len], nc->password, pass_len);

	is_success = open_close_data(FUJICMD_SET_SSID, true, data_size, data);
	free(data);
	return is_success;
}