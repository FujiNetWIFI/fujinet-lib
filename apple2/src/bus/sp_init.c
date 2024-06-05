#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

uint8_t read_memory(uint16_t address) {
	return *((volatile uint8_t *) address);
}


uint8_t sp_init() {
	const uint8_t sp_markers[] = {0x20, 0x00, 0x03, 0x00};
	uint16_t base;
	uint8_t i;
	bool match;
	uint8_t offset;
	uint16_t dispatch_address;

	// reset network id and is_init flag, we are going to rescan.
	sp_network = 0;
	sp_is_init = 0;

	for (base = 0xC700; base >= 0xC100; base -= 0x0100) {
		match = true;
		for (i = 0; i < 4; i++) {
			if (read_memory(base + i * 2 + 1) != sp_markers[i]) {
				match = false;
				break;
			}
		}

		if (match) {
			// If a match is found, calculate the dispatch function address
			offset = read_memory(base + 0xFF);
			dispatch_address = base + offset + 3;
			sp_dispatch_fn[0] = dispatch_address & 0xFF;
			sp_dispatch_fn[1] = dispatch_address >> 8;

			// now find and return the network id. it's stored in sp_network after calling sp_get_network_id.
			// we need to set sp_is_init to 1 to stop sp_get_network_id from calling init again and recursing.
			sp_is_init = 1;
			sp_get_network_id();
			if (sp_network != 0) {
				return sp_network;
			}
			// it failed to find a network on this SP device, so reset sp_is_init and reloop/exit
			sp_is_init = 0;
		}
	}

	// If no match is found, ensure dispatch function is cleared, sp_is_init is already 0, then return 0 for network not found.
	sp_dispatch_fn[0] = 0;
	sp_dispatch_fn[1] = 0;
	return 0;
}
