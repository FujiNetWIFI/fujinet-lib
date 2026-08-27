#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten)
{
	uint8_t params[3];

	params[0] = version;
	params[1] = ecc;
	params[2] = shorten ? 1 : 0;

	return open_close_data(FUJICMD_QRCODE_ENCODE, true, sizeof(params), params);
}
