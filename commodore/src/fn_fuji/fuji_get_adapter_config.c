#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_adapter_config(AdapterConfig *ac)
{
	int bytes_read;
	return open_read_close(FUJICMD_GET_ADAPTERCONFIG, &bytes_read, sizeof(AdapterConfig), (uint8_t *) ac);
}
