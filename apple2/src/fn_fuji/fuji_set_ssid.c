#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_set_ssid(NetConfig *nc)
{
	uint8_t nc_len = sizeof(NetConfig);

    sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_payload[0] = nc_len;
	sp_payload[1] = 0;
	memcpy(&sp_payload[2], nc, nc_len);

	sp_error = sp_control(sp_fuji_id, 0xFB);
	return sp_error == 0;

}
