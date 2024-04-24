#include <cmoc.h>

/**
 * @brief   Return Unit # from N: Devicespec
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Used by other net_ functions.
 * @param   devicespec The device spec to parse
 * @return  Unit number (0-255)
 */
unsigned char network_get_unit_from_devicespec(const char *devicespec)
{
    char *p;
    char tmp[256];
    unsigned char u = 0;

    memset(tmp,0,sizeof(tmp));
    
    strncpy(tmp,devicespec,sizeof(tmp));

    p = strtok(tmp,":");
    
    if (!p)
        return u;
    
    if (devicespec[0] != 'N')
        return u;

    if (devicespec[1] == ':')
        return u;

    u = (unsigned char)atoi(&p[1]); // scoot past N.

    return u;
}
