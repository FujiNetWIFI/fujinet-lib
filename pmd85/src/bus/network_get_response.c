#include <dw.h>
#include "fujinet-network.h"
#include "fujinet-fuji-pmd85.h"
#include "fujinet-network-pmd85.h"

/**
 * @brief Get response data from last Network command
 * @param unit Network device unit (1-255)
 * @param buf Target buffer 
 * @param len Length 
 * @return fujinet-network error code (see FN_ERR_* values)
 */

uint8_t network_get_response(uint8_t unit, uint8_t *buf, uint16_t len)
{
    /* Get response from Network Device OP_NET,unit */
    struct _getnetresponse
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t command;
        uint16_t len;
    } grc;

    grc.opcode = OP_NET;
    grc.unit = unit;
    grc.command = FUJICMD_GET_RESPONSE; 
    grc.len = (len << 8) | (len >> 8);

    bus_ready();
    dwwrite((uint8_t *)&grc, sizeof(grc));
    
    /* handle dwread() return value (update fn_device_error, it can be checked later, if necessary)
     * 0 -> BUS_ERROR(144) -> FN_ERR_IO_ERROR(1)
     * 1 -> BUS_SUCCESS(1) -> FN_ERR_OK(0)
     */
    return fn_error(dwread(buf, len) ? BUS_SUCCESS : BUS_ERROR);
}
