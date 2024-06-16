#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_get_ssid(NetConfig *net_config)
{
	int bytes_read;
	return open_read_close(FUJICMD_GET_SSID, true, &bytes_read, sizeof(NetConfig), (uint8_t *) net_config);
}
