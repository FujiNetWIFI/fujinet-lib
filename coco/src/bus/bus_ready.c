/**
 * @brief   Bus ready?
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Block until device ready.
 */

#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>


#define DWWRT_VEC_DRG 0xD941
#define DWREAD_VEC_DRG 0xD93F
#define DWWRT_VEC_COCO 0xFA00
#define DWREAD_VEC_COCO 0xF9FE

static gbDWInited = false;
// default to coco read/write if not inited.
dwvec dwreadvec = DWREAD_VEC_COCO;
dwvec dwwritevec = DWWRT_VEC_COCO;

// detection logic for dragon stolen from: https://exileinparadise.com/tandy_color_computer:keyscn2
//
const uint16_t* RESET = 0xFFFE;       // Address of MPU Reset Vector
const uint16_t VECCOCO = 0xA027;      // CoCo 1/2 reset vector value
const uint16_t VECDRGN = 0xB3B4;      // Dragon32/64 reset vector value
const uint16_t VECCOCO3 = 0x8C1B;     // CoCo3 reset vector value

void bus_ready(void)
{
    struct _readycmd
    {
        uint8_t opcode;
        uint8_t command;
    } rc;

    if (!gbDWInited)
    {
        if (*RESET==VECDRGN)
        {
            dwreadvec = DWREAD_VEC_DRG;
            dwwritevec = DWWRT_VEC_DRG;
        }
        gbDWInited = true;
    }

    uint8_t z=0, r;
    
    rc.opcode = OP_FUJI;
    rc.command = FUJICMD_READY;
    
    while (!z)
    {
        dwwrite((uint8_t *)&rc,sizeof(rc));
        z = dwread((uint8_t *)&r,sizeof(r));
    }  
}
