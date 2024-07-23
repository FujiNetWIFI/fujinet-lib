#include <stdbool.h>
#include <stdint.h>
#include "fujinet-network.h"
#include "fujinet-network-cbm.h"

uint8_t network_close(char* devicespec)
{
	uint8_t device_number;
	const char *after;
	uint8_t data_channel = getDeviceNumber(devicespec, &after) + CBM_DATA_CHANNEL_0;

	cbm_close(data_channel);
	network_num_channels_open--;

	// if all open data connections are closed, also close the command channel
	if (network_num_channels_open == 0) {
		cbm_close(CBM_CMD_CHANNEL);
	}
	return FN_ERR_OK;
}
