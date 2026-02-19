#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include "fujinet-clock.h"
#include "fujinet-network.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

static byte timebuf[6];

uint8_t clock_get_time(uint8_t* time_data, TimeFormat format)
{
    uint8_t result = FN_ERR_OK;

    byte cmd = 0x23; // DriveWire command to get time
    int year;
    
    if (format == SIMPLE_BINARY)
    {
        memset(&timebuf, 0, sizeof(timebuf));

        bus_ready();
        dwwrite((byte *)&cmd, 1);

        if(dwread(timebuf, 6))
        {
            // SIMPLE_BINARY: 7 bytes: [Y(century, e.g. 20), Y(hundreds, e.g. 24), M(1-12), D(1-31), H(0-23), M(0-59), S(0-59)]
            //                         Uses the current FN Timezone

            // timebuf[0]  + 1900 = year (e.g. 2024)
            year = (int )timebuf[0] + 1900u;
            time_data[0] = (uint8_t)(year / 100u);
            time_data[1] = (uint8_t)(year % 100u);
            time_data[2] = timebuf[1]; // Month (1-12)
            time_data[3] = timebuf[2]; // Day (1-31)
            time_data[4] = timebuf[3]; // Hour (0-23)
            time_data[5] = timebuf[4]; // Minute (0-59)
            time_data[6] = timebuf[5]; // Second (0-59)

            result = FN_ERR_OK;
        }
        else
        {
            result = FN_ERR_UNKNOWN;    
        }
    }
    else
    {
        // Only the simple binary format is supported for now, so return an error for other formats
        result = FN_ERR_BAD_CMD;
    }   

    return result;
}


