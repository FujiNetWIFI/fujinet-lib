#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-network-cbm.h"

uint8_t *jq_cmd = "jq,N,";

int16_t network_json_query(const char *devicespec, const char *query, char *s)
{
	// for CBM, we just send the "JQ" command with the appropriate channel id for devicespec, and add the query on the end
	const char *after;
	uint8_t *buffer;
	uint8_t data_channel;
	uint16_t bw;
	uint8_t conn;
	uint8_t err;
	uint8_t status_code;
	uint16_t bytes_read;

	uint8_t err_code = FN_ERR_OK;
	uint16_t buffer_len = strlen(query) + 6; // add string length of jq_cmd + 1 for terminating zero

	data_channel = getDeviceNumber(devicespec, &after) + CBM_DATA_CHANNEL_0;
	buffer = malloc(buffer_len);
	memset(buffer, 0, buffer_len);
	strcpy((char *) buffer, (const char *) jq_cmd);
	buffer[3] = data_channel + '0';	// overwrite N channel id
	strcpy(buffer+5, query);

	if (cbm_write(CBM_CMD_CHANNEL, buffer, buffer_len) != buffer_len)  {
		err_code = -FN_ERR_IO_ERROR;
	}

	// always free the buffer
	free(buffer);

	// don't continue if we failed to write the JQ query
	if (err_code != FN_ERR_OK) {
		return -err_code;
	}

	// call status so we can see the count - basic doesn't do this step...
	status_code = network_status(devicespec, &bw, &conn, &err);
	if (status_code != 0) return -status_code;

	// no data, or we have an EOF - TODO: should we affect the output here always? i.e. the empty string
	if (bw == 0 || err == 136) {
		*s = '\0';	// write a nul char for empty string and return 0 length
		return 0;
	}

	bytes_read = cbm_read(data_channel, s, bw);
	if (s[bytes_read] != '\0') {
		s[bytes_read] = '\0';
	}
	return bytes_read;
}
