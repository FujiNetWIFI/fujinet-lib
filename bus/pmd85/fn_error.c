#include "fujinet-network.h"
#include "fujinet-network-pmd85.h"
#include "fujinet-err.h"

uint8_t fn_error(uint8_t code)
{
	return (code == BUS_SUCCESS) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}
