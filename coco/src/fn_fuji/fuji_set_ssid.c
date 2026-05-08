#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_set_ssid(NetConfig *nc)
{
    struct _ss
    {
        uint8_t opcode;
        uint8_t cmd;
        char ssid[SSID_MAXLEN];
        char password[MAX_PASSWORD_LEN];
    } ss;

    ss.opcode = OP_FUJI;
    ss.cmd = FUJICMD_SET_SSID;
    memcpy(ss.ssid, nc->ssid, SSID_MAXLEN);
    memcpy(ss.password, nc->password, MAX_PASSWORD_LEN);
    
    bus_ready();
    dwwrite((uint8_t *)&ss, sizeof(ss));
    
    return !fuji_get_error();
}
