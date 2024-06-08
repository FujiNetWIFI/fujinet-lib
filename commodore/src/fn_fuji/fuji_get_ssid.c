#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_ssid(NetConfig *net_config)
{
	uint8_t pl[1];
	pl[0] = FUJICMD_GET_SSID;
	return open_read_close(1, pl, sizeof(NetConfig), (uint8_t *) net_config);
}
