#include <stdint.h>
#include <stdbool.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

uint8_t sp_printer_id = 0;

bool sp_find_printer(void) {
    int r = sp_find_device("PRINTER");
	if (r <= 0) {
		sp_printer_id = 0;
		return false;
	}
	sp_printer_id = (uint8_t) (r & 0xFF);
	return true;
}

uint8_t sp_get_printer_id(void)
{
	if (sp_printer_id != 0) {
		return sp_printer_id;
	}

	sp_find_printer();
	return sp_printer_id;
}