#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"
#include "fujinet-network-apple2.h"

uint8_t network_close(const char* devicespec) {
	uint8_t nw_device;
	uint16_t slen;
	uint16_t payload_len;

	// clear any errors
	fn_error(0);

	if (sp_is_init == 0) {
        nw_device = sp_init();
        if (nw_device == 0) {
            return fn_error(SP_ERR_IO_ERROR);
        }
    }

	sp_clr_payload();
	// set the unit this close is for
	network_set_unit(network_unit(devicespec));

	slen = strlen(devicespec);
	payload_len = slen + 1;  // + 1 for NUL string terminator
	sp_payload[0] = payload_len & 0xFF;
	sp_payload[1] = (payload_len >> 8) & 0xFF;

	strncpy((char *)&sp_payload[2], devicespec, slen);
	return sp_control(sp_network, 'C');
}