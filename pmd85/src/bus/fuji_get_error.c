#include <dw.h>
#include "fujinet-network.h"
#include "fujinet-fuji-pmd85.h"

/**
 * @brief Get the error code from last Fuji command
 * @return Error condition, true if error was received (or if failed to receive an error).
 */
bool fuji_get_error(void)
{
    struct _sendfujierror
    {
        uint8_t opcode;
        uint8_t cmd;
    } sec;

    uint8_t err;

    sec.opcode = OP_FUJI;
    sec.cmd = FUJICMD_SEND_ERROR;

    bus_ready();
    dwwrite((uint8_t *)&sec, sizeof(sec));

    /* handle dwread() return value (update fn_device_error, it can be checked later, if necessary)
     * 0 -> BUS_ERROR(144) -> FN_ERR_IO_ERROR(1) -> true
     * 1 -> err -> translate err to FN_ERR_xxx -> true to indicate that the error has occurred
     */
    return fn_error(dwread(&err, 1) ? err : BUS_ERROR) != FN_ERR_OK;
}
