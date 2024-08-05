#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"

extern uint8_t bad_unit();


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len) {
	uint16_t buf_len = 0;
	uint16_t sent_len = 0;
	uint8_t err;

	if (sp_network == 0) {
		return bad_unit();
	}
	if (len == 0 || buf == NULL) {
		return fn_error(SP_ERR_BAD_CMD);		
	}

	sp_nw_unit = network_unit(devicespec);

	while (len > 0) {
		buf_len = MIN(len, 512);
		// if we're always just sending amount we specify, should be no need to clear the buffer first even when under 512
		// if (buf_len < SP_PAYLOAD_SIZE) {
		// 	sp_clr_payload();
		// }
		memcpy(sp_payload, buf + sent_len, buf_len);
		err = sp_write_nw(sp_network, buf_len);
		if (err != 0) {
			return fn_error(err);
		}
		sent_len += buf_len;
		len -= buf_len;
	}

	// return OK
	return fn_error(SP_ERR_OK);

}