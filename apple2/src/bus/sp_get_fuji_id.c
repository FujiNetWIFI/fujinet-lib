#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

// only call sp_find_fuji if the id is not set
int8_t sp_get_fuji_id()
{
	if (sp_fuji_id != 0) {
		return sp_fuji_id;
	} else {
		return sp_find_fuji();
	}
}