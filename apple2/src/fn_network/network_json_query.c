#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"

extern uint8_t bad_unit(void);

int16_t network_json_query(char *devicespec, char *query, char *s) {
	uint8_t err = 0;
	uint16_t query_len = 0;
	uint16_t read_len = 0;
	char c;
	uint8_t offset = 0;

	fn_device_error = 0;
	sp_clr_payload();
	if (sp_network == 0) {
		return -bad_unit();
	}

	sp_nw_unit = network_unit(devicespec);
	query_len = strlen(query) + 1; // add 1 for nul
	sp_payload[0] = query_len & 0xFF;
	sp_payload[1] = query_len >> 8;

	strncpy((char *)sp_payload + 2, (const char *) query, query_len);
	err = sp_control_nw(sp_network, 'Q'); // perform JSON Query
	if (err != 0) {
		goto do_sp_error;
	}

	err = sp_status_nw(sp_network, 'S'); // get network status, which tells us bytes waiting
	if (err != 0) {
		goto do_sp_error;
	}

	read_len = sp_payload[0] + (sp_payload[1] << 8);
	if (read_len == 0) {
		// no data, set string to null char, and return 0 length. fn_error(0) returns 0, so this saves some bytes
		err = 0;
		goto do_sp_error;
	}

	err = sp_read_nw(sp_network, read_len);
	memcpy(s, sp_payload, read_len);

	// if last char is 0x9b, 0x0A or 0x0D, then set that char to nul, else just null terminate
	c = s[read_len - 1];
	if (c == 0x9B || c == 0x0A || c == 0x0D) {
		offset = 1;
	}
	s[read_len - offset] = '\0';

	// return the string length (minus any trailing EOL char)
	return read_len - offset;

do_sp_error:
	*s = '\0';
	return -fn_error(err);

}