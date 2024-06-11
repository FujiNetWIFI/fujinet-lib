#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_close_directory()
{
	return open_close(FUJICMD_CLOSE_DIRECTORY);
}
