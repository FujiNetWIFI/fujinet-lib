#include <stdint.h>
#include <stdbool.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

uint8_t sp_network = 0;

bool sp_find_network() {
    int r = sp_find_device("NETWORK");
	if (r <= 0) {
		sp_network = 0;
		return false;
	}
	sp_network = (uint8_t) (r & 0xFF);
	return true;
}

uint8_t sp_get_network_id()
{
	if (sp_network != 0) {
		return sp_network;
	}

	sp_find_network();
	return sp_network;
}