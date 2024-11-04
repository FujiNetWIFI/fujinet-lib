#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>

/**
 * @brief Get response data from last command
 * @param devid The device ID (0-255) 
 * @param buf Target buffer 
 * @param len Length 
 */

byte net_get_response(byte devid, byte *buf, int len)
{
    struct _getresponsecmd
    {
        byte opcode;
        byte id;
        byte command;
        int len;
    } grc;

    byte z=0;

    grc.opcode = OP_NET;
    grc.id = devid;
    grc.command = FUJICMD_GET_RESPONSE; 
    grc.len = len;

    bus_ready();
    dwwrite((byte *)&grc, sizeof(grc));
    dwread((byte *)buf, len);

    return z;
}
