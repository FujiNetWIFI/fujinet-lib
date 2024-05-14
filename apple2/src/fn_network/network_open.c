#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"

uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans) {
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

	if (sp_open(sp_network) != 0) {
		return fn_error(SP_ERR_IO_ERROR);
	}

	sp_clr_payload();
	slen = strlen(devicespec);
	payload_len = slen + 3;  // 2 for extra control bytes, 1 for NUL string terminator
	sp_payload[0] = payload_len & 0xFF;
	sp_payload[1] = (payload_len >> 8) & 0xFF;
	sp_payload[2] = mode;
	sp_payload[3] = trans;

	strncpy(&sp_payload[4], devicespec, slen);
	return sp_control(sp_network, 'O');
}