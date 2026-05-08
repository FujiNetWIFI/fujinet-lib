#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"
#include "fujinet-network-apple2.h"

void network_set_unit(uint8_t unit) {

	if (unit != sp_nw_unit) {
    sp_payload[0] = 1;
	sp_payload[1] = 0;
	sp_payload[2] = unit;

	if (!sp_control(sp_network, NETCMD_SET_CHANNEL))
	    sp_nw_unit = unit;
    }
}