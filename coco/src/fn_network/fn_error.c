#include <cmoc.h>
#include <coco.h>
#include "fujinet-network.h"
#include "fujinet-network-coco.h"

uint8_t fn_error(uint8_t code)
{
	fn_device_error = code;
	return (code == BUS_SUCCESS) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}
