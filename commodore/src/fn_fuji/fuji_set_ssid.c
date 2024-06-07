#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_set_ssid(NetConfig *nc)
{
	uint8_t err;
	uint8_t ssid_len;
	uint8_t pass_len;
	uint8_t data_size;
	uint8_t *data;

	ssid_len = strlen(nc->ssid);
	pass_len = strlen(nc->password);
	data_size = ssid_len + pass_len + 3;  // 1 for cmd byte, 2 for string nul chars

	data = malloc(data_size);
	memset(data, 0, data_size);

	data[0] = FUJICMD_SET_SSID;
	memcpy(&data[1], nc->ssid, ssid_len);
	memcpy(&data[2 + ssid_len], nc->password, pass_len);

	err = fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, data_size, data);
	free(data);
	if (err == 0) {
		cbm_close(FUJI_CMD_CHANNEL);
		return true;
	}
	return false;
}