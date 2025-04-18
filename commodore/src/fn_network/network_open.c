#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-network-cbm.h"

uint8_t cmd_channel_open[2] = { 0x01, 0x00 };

uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)
{
	uint8_t device_number;
	uint8_t data_channel;
	const char *url_part;
	char *open_raw;
	uint8_t url_len;
	uint8_t mem_size;
	uint8_t err_code = FN_ERR_OK;

	// only open command channel if we have no data channels open
	if (network_num_channels_open == 0) {
		// equivalent of 'OPEN 15,16,15, ""' but goes to FN to indicate fujinet-lib, and that to expect binary in writes
		if (fuji_cbm_open(CBM_CMD_CHANNEL, CBM_DEV_NETWORK, CBM_CMD_CHANNEL, 2, cmd_channel_open) != 0) {
			// TODO: how do we capture the errors for network, similar to FUJI device? Both share iecStatus values
			// status_error(ERROR_OPEN_CMD_FAILED, cmd);
			// TODO: this should call fn_error() with the return value from the open call
			return FN_ERR_IO_ERROR;
		}
	}

	// extract the network device id from the devicespec to form a data channel
	device_number = getDeviceNumber(devicespec, &url_part);
	data_channel = device_number + CBM_DATA_CHANNEL_0;


	// construct a binary open data command, prepending 0x01, mode, trans to the front of the URL part for firmware to detect we are sending additional parameters
	url_len = strlen(url_part);
	mem_size = url_len + 4; // 1 for marker, 2 for mode/trans bytes, 1 for string nul terminator
	open_raw = malloc(mem_size);
	memset(open_raw, 0, mem_size);
	open_raw[0] = 0x01;
	open_raw[1] = mode;
	open_raw[2] = trans;
	memcpy(open_raw + 3, url_part, url_len);
	open_raw[mem_size - 1] = '\0';

	// call cbm_open without the network device part, the id is converted to a channel number
	if (fuji_cbm_open(data_channel, CBM_DEV_NETWORK, data_channel, mem_size, open_raw) != 0) {
		// TODO: fix status, this should call fn_error(<value from cbm_open>)
		err_code = FN_ERR_IO_ERROR;
	} else {
		network_num_channels_open++;
	}
	free(open_raw);

	return err_code;
}
