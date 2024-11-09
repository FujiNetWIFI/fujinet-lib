#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"

extern uint8_t bad_unit(void);

// this is often called in a tight loop, so we won't clear the payload to help performance
uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err) {
	uint8_t err_status;

	if (sp_network == 0) {
		return bad_unit();
	}

	fn_device_error = 0;
	sp_nw_unit = network_unit(devicespec);

	err_status = sp_status_nw(sp_network, 'S'); // network status

	*bw = ((uint16_t)sp_payload[1] << 8) | sp_payload[0];
	*c = sp_payload[2];
	*err = sp_payload[3];

	return fn_error(err_status);
}