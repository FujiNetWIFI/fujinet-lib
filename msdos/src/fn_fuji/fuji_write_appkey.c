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
extern Appkey appkey;

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
    // Setup full appkey identifier
    appkey.creator_id = ak_creator_id;
    appkey.app_id = ak_app_id;
    appkey.key_id = key_id;
    appkey.open_mode = 1; // write
    appkey.reserved = 0;

    // Open app key
    int_f5_write(0x70,FUJICMD_OPEN_APPKEY,0,0,&appkey,sizeof(appkey));

    // Write data to app key
    return int_f5_write(0x70,FUJICMD_WRITE_APPKEY,64,0,data,64) > 0;
}
