#include <stdint.h>
#include "fujinet-network.h"

uint8_t t_cmd = 0x00;
uint8_t t_aux1 = 0x00;
uint8_t t_aux2 = 0x00;
char *t_devicespec = "n3:FOO://bar";
uint16_t t_use_aux = 0x0000;
void *t_buffer = 0x0000;
uint16_t t_len = 0x0000;

int main(void) {
    return network_ioctl(t_cmd, t_aux1, t_aux2, t_devicespec, t_use_aux, t_buffer, t_len);
}
