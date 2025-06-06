#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)
{
    struct _ho
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t output_type;
    } ho;

    ho.opcode = OP_FUJI;
    ho.cmd = FUJICMD_HASH_OUTPUT;
    ho.output_type = output_type;

    bus_ready();
    
    dwwrite((uint8_t *)&ho, sizeof(ho));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)s, len);
}
