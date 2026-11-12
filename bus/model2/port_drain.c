#include "portio.h"

// Bounded, so a port that always has data can't hold the call forever.
#define DRAIN_MAX 1024

void port_drain(void)
{
    uint16_t n;

    for (n = 0; n < DRAIN_MAX && port_getc_timeout(1) >= 0; n++)
        ;
}
