#include <stdint.h>
#include <stdbool.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

uint8_t sp_modem_id = 0;

bool sp_find_modem(void) {
    int r = sp_find_device("MODEM");
	if (r <= 0) {
		sp_modem_id = 0;
		return false;
	}
	sp_modem_id = (uint8_t) (r & 0xFF);
	return true;
}

uint8_t sp_get_modem_id(void)
{
	if (sp_modem_id != 0) {
		return sp_modem_id;
	}

	sp_find_modem();
	return sp_modem_id;
}