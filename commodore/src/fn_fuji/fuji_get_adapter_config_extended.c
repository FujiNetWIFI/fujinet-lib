#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
{
	uint8_t pl[1];
	pl[0] = FUJICMD_GET_ADAPTERCONFIG_EXTENDED;
	return open_read_close(1, pl, sizeof(AdapterConfigExtended), (uint8_t *) ac);
}
