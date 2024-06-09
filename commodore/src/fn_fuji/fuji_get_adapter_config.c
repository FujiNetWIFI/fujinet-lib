#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_adapter_config(AdapterConfig *ac)
{
	uint8_t pl[1];
	pl[0] = FUJICMD_GET_ADAPTERCONFIG;
	return open_read_close(1, pl, sizeof(AdapterConfig), (uint8_t *) ac);
}
