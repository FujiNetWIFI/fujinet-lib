#include <stdint.h>
#include <string.h>
#include <cbm.h>
#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_open_directory(uint8_t hs, char *path_filter)
{
    // memset(response, 0, sizeof(response));

    response[0] = FUJICMD_OPEN_DIRECTORY;
    response[1] = hs;
    strncpy(&response[2], path_filter, sizeof(response) - 2);

    cbm_open(LFN, DEV, SAN, response);
    cbm_close(LFN);
}
