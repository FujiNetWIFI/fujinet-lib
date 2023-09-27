#include "fujinet-network.h"

char *my_devicespec = "n3:FOO://bar";
uint8_t my_dstats = 0x40;
uint16_t my_dbuf = 0x4abc;
uint16_t my_dbyt = 0x1234;

int main(void) {
    return network_ioctl('X', 1, 2, my_devicespec, my_dstats, my_dbuf, my_dbyt);
}
