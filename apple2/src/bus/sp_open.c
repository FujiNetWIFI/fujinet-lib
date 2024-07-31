#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

int8_t sp_open(uint8_t dest) {
	sp_cmdlist[1] = dest;
	sp_cmdlist[0] = SP_OPEN_PARAM_COUNT;
	sp_cmdlist[2] = sp_nw_unit;

	return sp_dispatch(SP_CMD_OPEN);
}