#include <stdint.h>
#include "fujinet-network.h"

uint8_t t_cmd = 0x00;
uint8_t t_aux1 = 0x00;
uint8_t t_aux2 = 0x00;
char *t_devicespec = "n3:FOO://bar";
uint16_t t_use_aux = 0x0000;
void *t_buffer = 0x0000;
uint16_t t_len = 0x0000;
uint8_t t_sp_network = 1;
uint8_t t_sp_is_init = 0;

extern uint8_t sp_network;
extern uint8_t sp_is_init;

int main(void) {
    // for testing, use t_sp_is_init for _sp_is_init value
    sp_is_init = t_sp_is_init;

    // for testing, if t_sp_network is 0, we set sp_network to 0 so we can fake not finding the network id
    if (t_sp_network != 1) {
        sp_network = 0;
    }
    return network_ioctl(t_cmd, t_aux1, t_aux2, t_devicespec, t_use_aux, t_buffer, t_len);
}
