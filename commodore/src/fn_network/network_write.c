#include <stdbool.h>
#include <stdint.h>
#include "fujinet-network.h"
#include "fujinet-network-cbm.h"

uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len)
{
	const char *after;
	uint8_t data_channel;
	uint8_t err_code = FN_ERR_OK;
	data_channel = getDeviceNumber(devicespec, &after) + CBM_DATA_CHANNEL_0;
	if (cbm_write(data_channel, buf, len) != len)  {
		err_code = FN_ERR_IO_ERROR;
	}
	return err_code;
}
