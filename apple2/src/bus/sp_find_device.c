#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

int compare_padded_string(const char *padded, const char *name) {
    const size_t padded_length = 16;        // device names are padded to 16 chars with spaces
    size_t name_length = strlen(name);
    size_t i;

    // Compare the part of the strings that overlaps with 'name'
    for (i = 0; i < name_length; i++) {
        if (i >= padded_length || padded[i] != name[i]) {
            return -1; // Strings differ
        }
    }

    // Ensure the rest of 'padded' is just spaces, if 'name' is shorter than 'padded_length'
    for (; i < padded_length; i++) {
        if (padded[i] != ' ') {
            return -1; // 'padded' contains non-space characters where 'name' has ended
        }
    }

    return 0; // Strings match
}

int sp_find_device(char *name) {
    int8_t deviceCount;
	int8_t id;
	uint8_t err;

    // check we've run init already
    if (sp_is_init == 0) {
        // This returns 0 for network device not found as it initialises and scans for a network device.
        err = sp_init();
        if (err == 0) {
            return 0;
        }
    }

    // Initiate device discovery to get the count
    err = sp_status(0, 0);
	if (err != 0) return -err;

    deviceCount = sp_payload[0];
	if (deviceCount <= 0) return deviceCount;

    for (id = 1; id <= deviceCount; id++) {
        err = sp_status(id, 3);
		if (err != 0) return -err;

        if (compare_padded_string((const char *)&sp_payload[5], name) == 0) {
            return id;
        }
    }

    return 0; // Return 0 if not found
}