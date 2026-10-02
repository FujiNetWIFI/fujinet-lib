#ifndef FUJINET_PET_CHARMAP_H
#define FUJINET_PET_CHARMAP_H

// For DATA transfers, we want to undo some of the charmap settings in CC65.
// So map the characters in the range 5b-60, 7b-7f back to themselves, as we want to send them as-is to FujiNet.
// This undoes some "look alike" character that move into 0x80+ range, which in FN are converted to 3 byte utf-8 chars incorrectly.

#pragma charmap (0x5B, 0x5B)
#pragma charmap (0x5C, 0x5C)
#pragma charmap (0x5D, 0x5D)
#pragma charmap (0x5E, 0x5E)
#pragma charmap (0x5F, 0x5F)
#pragma charmap (0x60, 0x60)
#pragma charmap (0x7B, 0x7B)
#pragma charmap (0x7C, 0x7C)
#pragma charmap (0x7D, 0x7D)
#pragma charmap (0x7E, 0x7E)
#pragma charmap (0x7F, 0x7F)

#endif // FUJINET_PET_CHARMAP_H