#include <stdint.h>
#include "fujinet-network.h"

char *t_devicespec = "n3:FOO://bar";
uint8_t t_cmd = 0x00;
uint8_t t_aux1 = 0x00;
uint8_t t_aux2 = 0x00;

int main(void) {
    // bad call, not enough args
    return network_ioctl(t_cmd, t_aux1, t_aux2, t_devicespec);
}
