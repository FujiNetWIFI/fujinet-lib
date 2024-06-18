#if defined(_CMOC_VERSION_)

#include <cmoc.h>
#include <coco.h>

#else

#include <stdbool.h>
#include <stdint.h>

#endif // _CMOC_VERSION_

#include "fujinet-fuji.h"

bool fuji_hash_add(uint8_t *data, uint16_t length)
{
	// internally, char * and uint8_t * aren't going to matter as the data isn't interpret in anyway in transferring to FujiNet
	return fuji_hash_input((char *) data, length);
}