#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_ssid(NetConfig *net_config)
{
	int bytes_read;
	uint8_t pl[1];
	pl[0] = 0xFE;

	if (fuji_cbm_open(FUJI_CMD_CHANNEL, FUJI_CBM_DEV, FUJI_CMD_CHANNEL, 1, (uint8_t *) pl) != 0) {
		return false;
	}

	bytes_read = cbm_read(FUJI_CMD_CHANNEL, net_config, sizeof(NetConfig));
	cbm_close(FUJI_CMD_CHANNEL);
	return bytes_read > 0;
}
