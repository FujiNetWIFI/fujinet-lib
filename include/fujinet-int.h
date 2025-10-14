#ifndef FUJINET_INT_H
#define FUJINET_INT_H

#ifdef _CMOC_VERSION_
    #include <cmoc.h>
    #include <coco.h>

    #ifndef bool
    #define bool _FNBool
    typedef unsigned char _FNBool;
    #endif /* bool */

    #define true  1
    #define false 0

#else /* ! _CMOC_VERSION */
    #include <stddef.h>
    #include <stdbool.h>
    #include <stdint.h>
#endif /* _CMOC_VERSION_ */

#endif /* FUJINET_INT_H */
