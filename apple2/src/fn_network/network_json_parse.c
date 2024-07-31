#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"

extern uint8_t bad_unit();

uint8_t network_json_parse(char *devicespec) {
	uint8_t err = 0;

	fn_device_error = 0;
	sp_clr_payload();
	if (sp_network == 0) {
		return bad_unit();
	}

	sp_nw_unit = network_unit(devicespec);
	sp_payload[0] = 1;		// len = 1 byte, payload[0] already 0
	sp_payload[2] = 1; 		// json mode
	
	err = sp_control(sp_network, CHANNEL_MODE_JSON);
	if (err != 0) {
		return fn_error(err);
	}

	// do JSON parse
	return fn_error(sp_control(sp_network, 'P'));
}