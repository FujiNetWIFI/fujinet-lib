#include "fujinet-network.h"

#define PIA2_REG0 ((uint8_t *) 0xFF20)
#define PIA2_REG1 (PIA2_REG0 + 1)

/**********************************************************************
 We need to know if we're using a serial port or a Becker port. Becker
 port currently has no 1 bit status to see if network data/error is
 available. This will probably change with FujiNet bus cart, but
 emulators will still not support proceed status.

 ----------------------------------------
 Filename                       | 0x11C
 ----------------------------------------
 hdbdw3cc1.rom                  | 93
 hdbdw3cc2.rom                  | BF
 hdbdw3cc3.rom                  | C8
 hdbdw3dgn.rom                  | C9
 hdbdw3bck.rom                  | 47
 hdbdw3bc3.rom                  | 50
**********************************************************************/

typedef enum {
  COCO_1_BB     = 0x93,
  COCO_2_BB     = 0xBF,
  COCO_3_BB     = 0xC8,
  DRGN_BB       = 0xC9,
  COCO_BECKER   = 0x47,
  COCO_3_BECKER = 0x50,
} HDB_TYPE;

#define HDB_DOS_ADDRESS 0xC000
#define HDB_ID_OFFSET   0x11C
#define HDB_GET_ID() (*((uint8_t *) (HDB_DOS_ADDRESS + HDB_ID_OFFSET)))

static bool _did_cd_init = false;

static void coco_init_cd(void)
{
  uint8_t val;

  val = *PIA2_REG1;
  val &= 0xFC; // Disable FIRQ, flag on falling edge (CD off to on)
  val |= 2; // flag on rising edge (CD on to off)
  *PIA2_REG1 = val;
  (void) *PIA2_REG0; // Clear the CD flag

  _did_cd_init = true;
  return;
}

bool network_has_proceed(void)
{
  HDB_TYPE type = HDB_GET_ID();


  if (type == COCO_1_BB || type == COCO_2_BB || type == COCO_3_BB)
    return true;
  return false;
}

bool network_proceed(void)
{
  if (!_did_cd_init)
    coco_init_cd();

  if ((*PIA2_REG1) & 0x80) {
    while ((*PIA2_REG1) & 0x80)
      (void) *PIA2_REG0; // Clear the CD flag
    return true;
  }

  return false;
}
