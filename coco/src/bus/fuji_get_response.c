#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>

/**
 * @brief Get response data from last Fuji command
 * @param buf Target buffer (needs to be at least len)
 * @param len Length 
 */

uint8_t fuji_get_response(uint8_t *buf, int len)
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
    
    /* Return 0 on a successful read (dwread returns 1) */
    return !dwread((byte *)buf, len);
}
