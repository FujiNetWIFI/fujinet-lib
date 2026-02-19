#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_generate_guid(char *buffer)
{
	int bytes_read;
	return open_read_close(FUJICMD_GENERATE_GUID, true, &bytes_read, MAX_GUID_LEN, (uint8_t *) buffer);
}
