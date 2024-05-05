#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"

bool fuji_open_directory(uint8_t hs, char *path_filter)
{
	// not implementing this version in CBM, use fuji_open_directory2 with separate path and filter
	return false;
}
