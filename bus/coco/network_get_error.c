#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include "fujinet-network.h"
#include "fujinet-fuji-coco.h"
#include "fujinet-network-coco.h"

/**
 * @brief Get the error code from last Network command
 * @param unit Network device unit (1-255)
 * @return fujinet-network error code (see FN_ERR_* values)
 */

uint8_t network_get_error(uint8_t unit)
{
    /* Get error code from Network Device OP_NET,unit */
    struct _sendneterror
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t command;
        uint16_t auxs;
    } sec;

    uint8_t err;

    sec.opcode = OP_NET;
    sec.unit = unit;
    sec.command = FUJICMD_SEND_ERROR; 
    sec.auxs = 0;

    bus_ready();
    dwwrite((byte *)&sec, sizeof(sec));
    
    /* handle dwread() return value (update fn_device_error, it can be checked later, if necessary)
     * 0 -> BUS_ERROR(144) -> FN_ERR_IO_ERROR(1)
     * 1 -> translate err to FN_ERR_xxx
     */
    return fn_error(dwread((byte *)&err, 1) ? err : BUS_ERROR);
}
