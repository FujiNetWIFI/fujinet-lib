#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
	int bytes_read;
	return open_read_close_data_2(FUJICMD_READ_DIR_ENTRY, &bytes_read, maxlen, aux2, maxlen, (uint8_t *) buffer);
}
