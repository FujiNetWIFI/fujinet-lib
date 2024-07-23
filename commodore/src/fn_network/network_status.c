#include <stdbool.h>
#include <stdint.h>
#include "fujinet-network.h"
#include "fujinet-network-cbm.h"

uint8_t *status_cmd = "statusb,N";

// we only really need the device id, not the full devicespec in all network_status calls.
uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
{
	// if we do a read on the status_cmd channel, we get the status for the specified devicespec, but we have to tell FN which device we want the status for first
	const char *after;
	uint8_t status[4];
    uint16_t temp_bw = 0;
    uint8_t temp_c = 0;
	uint8_t temp_err = 0;
	char *ptr;

	// a "binary status" command, requires up to date firmware
	uint8_t data_channel = getDeviceNumber(devicespec, &after) + CBM_DATA_CHANNEL_0;
	status_cmd[8] = data_channel + '0';   // overwrite with network device id

	if (cbm_write(CBM_CMD_CHANNEL, status_cmd, 9) != 9)  {
		return FN_ERR_IO_ERROR;
	}

	// now the command channel was updated with the correct data channel, let's do a read on the command channel and set the values from it
	if (cbm_read(CBM_CMD_CHANNEL, status, 4) != 4) {
		return FN_ERR_IO_ERROR;
	}

	if (bw) {
        *bw = (uint16_t)(status[1] << 8) | status[0]; // Combine the first two bytes for bw
    }
    if (c) {
        *c = status[2]; // The third byte is directly assigned to c
    }
    if (err) {
        *err = status[3]; // The fourth byte is directly assigned to err
    }

	return FN_ERR_OK;
}
