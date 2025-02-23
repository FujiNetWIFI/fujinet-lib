#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>
#include <string.h>

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;
#define BUFFER_SIZE 64

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
    // Minimally viable appkey support for 64 length (DEFAULT) app keys

    struct _ra
    {
        uint8_t opcode;
        uint8_t cmd;
        uint16_t creator;
        uint8_t app;
        uint8_t key;
        uint8_t mode;
        uint8_t reserved;
    } ra;

    struct _wa
    {
        uint8_t opcode;
        uint8_t cmd;
        uint16_t size;
        char data[BUFFER_SIZE];
    } wa;

    if (ak_creator_id == 0) {
		return false;
	}

    ra.opcode = OP_FUJI;
    ra.cmd = FUJICMD_OPEN_APPKEY;
    ra.key = key_id;
    ra.creator = (ak_creator_id << 8) | (ak_creator_id >> 8);
    ra.app = ak_app_id;
    ra.key = key_id;
    ra.mode = 1; // Write mode

    bus_ready();
    dwwrite((uint8_t *)&ra, sizeof(ra));
    
    wa.opcode = OP_FUJI;
    wa.cmd = FUJICMD_WRITE_APPKEY;
    wa.size = (BUFFER_SIZE << 8) | (BUFFER_SIZE >> 8);
    memcpy(&wa.data, data, BUFFER_SIZE);

    bus_ready();
    dwwrite((uint8_t *)&wa, sizeof(wa));

    return true;
}
