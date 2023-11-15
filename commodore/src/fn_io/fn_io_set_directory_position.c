#include <stdint.h>
#include <string.h>
#include <cbm.h>
#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_set_directory_position(uint16_t pos)
{
    memset(response, 0, sizeof(response));

    response[0] = FUJICMD_SET_DIRECTORY_POSITION;
    response[1] = pos & 0xFF;
    response[2] = pos >> 8;

    cbm_open(LFN, DEV, SAN, response);
    cbm_close(LFN);
}
