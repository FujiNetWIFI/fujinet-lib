#include <stdint.h>
#include <string.h>

#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_create_new(NewDisk *new_disk)
{
	if (sp_get_fuji_id() == 0) {
		return false;
	}

	sp_payload[0] = sizeof(NewDisk) & 0xFF;
	sp_payload[1] = (sizeof(NewDisk) & 0xFF00) >> 8;

	sp_payload[2] = new_disk->hostSlot;
	sp_payload[3] = new_disk->deviceSlot;
	sp_payload[4] = new_disk->createType;
	memcpy(&sp_payload[5], &new_disk->numBlocks, sizeof(uint32_t));
	memcpy(&sp_payload[9], &new_disk->filename[0], sizeof(new_disk->filename));

	sp_error = sp_control(sp_fuji_id, FUJICMD_NEW_DISK);
	return sp_error == 0;
}
