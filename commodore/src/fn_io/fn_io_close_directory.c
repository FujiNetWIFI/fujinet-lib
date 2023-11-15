#include <stdint.h>
#include <string.h>
#include <cbm.h>
#include "fujinet-io.h"
#include "fn_data.h"

void fn_io_close_directory(void)
{
    memset(response, 0, sizeof(response));

    response[0] = FUJICMD_CLOSE_DIRECTORY;

    cbm_open(LFN, DEV, SAN, response);
    cbm_close(LFN);
}
