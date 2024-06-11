#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_put_host_slots(HostSlot *h, size_t size)
{
	uint16_t payload_size = sizeof(HostSlot) * size;
	uint8_t *hs_data;
	bool ret;

	hs_data = malloc(payload_size);
	if (hs_data == NULL) {
		status_error();
		return false;
	}

	memcpy(hs_data, (uint8_t *) h, payload_size);

	ret = open_close_data(FUJICMD_WRITE_HOST_SLOTS, payload_size, hs_data);
	free(hs_data);
	return ret;
}
