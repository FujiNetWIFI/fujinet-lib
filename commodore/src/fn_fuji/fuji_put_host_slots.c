#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_put_host_slots(HostSlot *h, size_t size)
{
	uint16_t payload_size = sizeof(HostSlot) * size + 1;
	uint8_t *hs_data;
	bool ret;

	hs_data = malloc(payload_size);
	if (hs_data == NULL) {
		return false;
	}

	hs_data[0] = FUJICMD_WRITE_HOST_SLOTS;
	memcpy(&hs_data[1], (uint8_t *) h, payload_size - 1);

	ret = open_close(payload_size, hs_data);
	free(hs_data);
	return ret;
}
