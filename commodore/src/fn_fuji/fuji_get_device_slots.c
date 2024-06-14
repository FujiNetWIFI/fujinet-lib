#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_device_slots(DeviceSlot *d, size_t size)
{
	uint8_t *ds_data;
	int bytes_read;
	bool is_success;
	int max_ds_data_size = 8 * sizeof(DeviceSlot);

	// see comments in fuji_get_host_slots().
	ds_data = malloc(max_ds_data_size);
	if (ds_data == NULL) {
		status_error();
		return false;
	}

	is_success = open_read_close(FUJICMD_READ_DEVICE_SLOTS, &bytes_read, max_ds_data_size, ds_data);
	if (!is_success || (bytes_read != (sizeof(DeviceSlot) * size))) {
		// we didn't get the right amount of data back.
		free(ds_data);
		return false;
	}

	// matched the size expecting, so copy it into the target array, and release the memory we used as a buffer
	memcpy(d, ds_data, bytes_read);
	free(ds_data);

	return true;
}
