#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;
#define BUFFER_SIZE 64

bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
{

	if (ak_creator_id == 0) {
		return false;
	}

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

    ra.opcode = OP_FUJI;
    ra.cmd = FUJICMD_OPEN_APPKEY;
    ra.key = key_id;
    ra.creator = ak_creator_id;
    ra.app = ak_app_id;
    ra.key = key_id;
    ra.mode = 0;

    bus_ready();
    dwwrite((uint8_t *)&ra, sizeof(ra));
    
    ra.opcode = OP_FUJI;
    ra.cmd = FUJICMD_READ_APPKEY;
    
    bus_ready();
    dwwrite((uint8_t *)&ra, 2);

    if (bus_error(OP_FUJI)) {
        return false;
    }

    bus_get_response(OP_FUJI,(uint8_t *)data,BUFFER_SIZE+2);
    *count = *(uint16_t*)data;
    memcpy(data,data+2, BUFFER_SIZE);
    return true;
}
