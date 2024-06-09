#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
	uint8_t pl[3];
	pl[0] = FUJICMD_GET_ADAPTERCONFIG;
	pl[1] = maxlen;
	pl[2] = aux2;
	return open_read_close(3, pl, maxlen, (uint8_t *) buffer);
}
