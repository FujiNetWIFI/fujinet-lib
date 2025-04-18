#include <stdbool.h>
#include <stdint.h>
#include "fujinet-network.h"
#include "fujinet-network-cbm.h"

uint8_t *cmd_mode = "chmode,N,json";
uint8_t *cmd_parse = "jsonparse,N"; 

uint8_t network_json_parse(const char *devicespec)
{
	// for CBM, we need to set the channel mode to json, then send "JSONPARSE" command with the appropriate channel id for devicespec
	const char *after;
	uint8_t data_channel = getDeviceNumber(devicespec, &after) + CBM_DATA_CHANNEL_0;
	uint8_t dc_letter = data_channel + '0';

	cmd_mode[7] = dc_letter;
	if (cbm_write(CBM_CMD_CHANNEL, cmd_mode, 13) != 13) {
		return FN_ERR_IO_ERROR;
	}

	cmd_parse[10] = dc_letter;
	if (cbm_write(CBM_CMD_CHANNEL, cmd_parse, 11) != 11) {
		return FN_ERR_IO_ERROR;
	}

	return FN_ERR_OK;
}
