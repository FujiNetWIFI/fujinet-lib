#include <stdbool.h>
#include <stdint.h>
#include "fujinet-network.h"
#include "fujinet-network-cbm.h"


uint8_t *scm_cmd = "M,c,0,m";

uint8_t network_http_set_channel_mode(char *devicespec, uint8_t mode)
{
	// send "M,<channel>,0,<mode>"
	const char *after;
	uint8_t data_channel = getDeviceNumber(devicespec, &after) + CBM_DATA_CHANNEL_0;

	scm_cmd[2] = data_channel + '0';
	scm_cmd[6] = mode + '0';

	if (cbm_write(CBM_CMD_CHANNEL, scm_cmd, 7) != 7) {
		return FN_ERR_IO_ERROR;
	}

	return FN_ERR_OK;
}
