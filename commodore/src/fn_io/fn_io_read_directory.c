#include <stdint.h>
#include <string.h>
#include <cbm.h>
#include "fujinet-io.h"
#include "fn_data.h"

// buffer must be at least 1024 chars
char *fn_io_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
    memset(buffer, 0, 1024);

    response[0] = FUJICMD_READ_DIR_ENTRY;
    response[1] = maxlen;
    response[2] = aux2;

    cbm_open(LFN, DEV, SAN, response);
    cbm_read(LFN, buffer, 1024);
    return buffer;
}
