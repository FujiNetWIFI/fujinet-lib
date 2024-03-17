#include <stdint.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

bool fn_io_error()
{
    // a2 config just returns "sp_error", but that's an int. TYPES DAMN IT
    return sp_error != 0;
}
