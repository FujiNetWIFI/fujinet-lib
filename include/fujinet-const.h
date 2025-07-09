#ifndef FUJINET_CONST_H
#define FUJINET_CONST_H

#define FILE_MAXLEN    36
#define SSID_MAXLEN    33 /* 32 + NULL */
#define MAX_APPKEY_LEN 64
#define MAX_FILENAME_LEN 256

// FIXME - use dynamic length
#define MAX_APPKEY_LEN 64

#ifdef __CBM__
    #define MAX_PASSWORD_LEN 65
#else
#define MAX_PASSWORD_LEN 64
#endif

#endif /* FUJINET_CONST_H */
