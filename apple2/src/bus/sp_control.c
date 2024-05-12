#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

int8_t sp_control(uint8_t dest, uint8_t ctrlcode) {
	uintptr_t payload_address;

	sp_cmdlist[4] = ctrlcode;

	sp_cmdlist[0] = SP_STATUS_PARAM_COUNT;
	sp_cmdlist[1] = dest;

	payload_address = (uintptr_t)(&sp_payload[0]);
	sp_cmdlist[2] = payload_address & 0xFF;
	sp_cmdlist[3] = (payload_address >> 8) & 0xFF;

	return sp_dispatch(SP_CMD_CONTROL);
}