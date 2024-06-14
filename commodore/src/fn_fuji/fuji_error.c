#include <stdbool.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

bool fuji_error()
{
	// this assumes the status has been fetched already.
	// TODO: may want to do similar to "get_fuji_status" and reverse the value,
	// but The problem with fetching the status is it could affect the status if it fails to work
	return _fuji_status.value.error != 0;
}
