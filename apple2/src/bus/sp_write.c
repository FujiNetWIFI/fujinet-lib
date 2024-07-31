#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

int8_t sp_write(uint8_t dest, uint16_t len) {
	uintptr_t payload_address;

	sp_cmdlist[4] = len & 0xFF;
	sp_cmdlist[5] = (len >> 8) & 0xFF;

	// SP WRITE can technically take an address, certainly it's sent when doing SP over SLIP, but
	// in network context it isn't used, so we will stuff the UNIT id of the request into byte 7
	sp_cmdlist[6] = sp_nw_unit;

	sp_cmdlist[0] = SP_WRITE_PARAM_COUNT;
	sp_cmdlist[1] = dest;

	payload_address = (uintptr_t)(&sp_payload[0]);
	sp_cmdlist[2] = payload_address & 0xFF;
	sp_cmdlist[3] = (payload_address >> 8) & 0xFF;

	sp_dispatch(SP_CMD_WRITE);
	return sp_error;
}