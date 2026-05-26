#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_HOSTS 8
#if defined(BUILD_COCO)
#define MAX_DISKS 4
#elif defined(BUILD_APPLE2)
#define MAX_DISKS 10
#else
#define MAX_DISKS 8
#endif

/* Network device specs used across tests */
#define NET_DEVICESPEC  "N1:HTTPS://fujinet.online/"
#define NET_JSON_URL    "N1:HTTPS://httpbin.org/json"
#define NET_POST_URL    "N1:HTTPS://httpbin.org/post"
#define NET_DELETE_URL  "N1:HTTPS://httpbin.org/delete"
#define NET_TCP_SPEC    "N1:TCP://tcpbin.com:4242/"

#endif /* CONSTANTS_H */
