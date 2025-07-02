#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus.h"

// TODO: sort this out! To get status, we need a code for
// every different device as there's no payload to specify which device.
bool fuji_get_device_enabled_status(uint8_t d)
{
	return true;
}
