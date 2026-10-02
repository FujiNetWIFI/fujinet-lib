#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fujinet-network.h"
#include "fujinet-fuji-coco.h"

/**
 * @brief Get response data from last Fuji command
 * @param buf Target buffer (needs to be at least len)
 * @param len Length 
 * @return Success status, true if the response was received successfully.
*/

bool fuji_get_response(uint8_t *buf, int len)
{
    /* Get response from Fuji Device OP_FUJI */
    struct _getfujiresponse
    {
        uint8_t opcode;
        uint8_t command;
    } grc;

    grc.opcode = OP_FUJI;
    grc.command = FUJICMD_GET_RESPONSE; 

    bus_ready();
    dwwrite((byte *)&grc, sizeof(grc));
    
    /* handle dwread() return value (update fn_device_error, it can be checked later, if necessary)
     * 0 -> BUS_ERROR(144) -> FN_ERR_IO_ERROR(1) -> false
     * 1 -> BUS_SUCCESS(1) -> FN_ERR_OK(0) -> true
     */
    return !fn_error(dwread((byte *)buf, len) ? BUS_SUCCESS : BUS_ERROR);
}
