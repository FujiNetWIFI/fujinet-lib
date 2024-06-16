#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
{
	int bytes_read;
	return open_read_close(FUJICMD_GET_ADAPTERCONFIG_EXTENDED, true, &bytes_read, sizeof(AdapterConfigExtended), (uint8_t *) ac);
}
