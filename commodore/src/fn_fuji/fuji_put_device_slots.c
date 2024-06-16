#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_put_device_slots(DeviceSlot *d, size_t size)
{
	uint16_t payload_size = sizeof(DeviceSlot) * size;
	uint8_t *ds_data;
	bool ret;

	ds_data = malloc(payload_size);
	if (ds_data == NULL) {
		status_error(ERROR_MALLOC_FAILED, FUJICMD_WRITE_DEVICE_SLOTS);
		return false;
	}

	memcpy(ds_data, (uint8_t *) d, payload_size);

	ret = open_close_data(FUJICMD_WRITE_DEVICE_SLOTS, true, payload_size, ds_data);
	free(ds_data);
	return ret;
}
