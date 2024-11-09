#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_error(void)
{
    // a2 config just returns "sp_error", but that's an int. TYPES DAMN IT
    return sp_error != 0;
}
