#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

typedef struct _appkey
{
    unsigned short creator_id;
    unsigned char app_id;
    unsigned char key_id;
    unsigned char open_mode;
    unsigned char reserved;
} Appkey;

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern uint8_t ak_appkey_size; 
Appkey appkey;

bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
{
    short result;

    // Setup full appkey identifier
    appkey.creator_id = ak_creator_id;
    appkey.app_id = ak_app_id;
    appkey.key_id = key_id;
    appkey.open_mode = 0; // read
    appkey.reserved = 0;

    // Open app key
    int_f5_write(0x70,FUJICMD_OPEN_APPKEY,0,0,&appkey,sizeof(appkey));

    // Read app key data
    if (int_f5_read(0x70,FUJICMD_READ_APPKEY,0,0,data,66)) {
        *count = *(uint16_t*)data;
        memcpy(data,data+2, 64);
        return true;
    } else {
        count=0;
    }
    return false;

}
