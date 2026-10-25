#ifndef CONSTANTS_H
#define CONSTANTS_H

// OPEN_TRANS_ translates from the FujiNet bus's native line ending, so this
// must match nativeEOL() for the platform's bus: CR LF on RS232, CR by default.
// cc65's '\n' is already native on the Atari and C64.
#if defined(FUJI_PLATFORM_MSDOS) || defined(FUJI_PLATFORM_MODEL2) \
  || defined(FUJI_PLATFORM_MSX)
#define BASIC_LINE_ENDING "\r\n"
#elif defined(FUJI_PLATFORM_APPLE2) || defined(FUJI_PLATFORM_COCO) \
  || defined(FUJI_PLATFORM_ADAM) || defined(FUJI_PLATFORM_PMD85) \
  || defined(FUJI_PLATFORM_H89) || defined(FUJI_PLATFORM_LYNX)
#define BASIC_LINE_ENDING "\r"
#elif defined(FUJI_PLATFORM_ATARI) || defined(FUJI_PLATFORM_C64)
#define BASIC_LINE_ENDING "\n"
#else
#error "BASIC_LINE_ENDING is not defined for this platform"
#endif

#define ECHO_MSG "... FujiNet integration test"

/* Network device specs used across tests */
#if 1
#define NET_TCP_SPEC    "N1:TCP://tcpbin.com:4242/"
#define NET2_ECHO       "N2:TCP://tcpbin.com:4242/"
#define NET_TELNET_SPEC "N1:TELNET://tcpbin.com:4242/"
#define NET2_TELNET     "N2:TELNET://tcpbin.com:4242/"
#else
// socat -v TCP-LISTEN:4242,fork EXEC:'cat'
#define NET_TCP_SPEC    "N1:TCP://10.4.0.1:4242/"
#define NET2_ECHO       "N2:TCP://10.4.0.1:4242/"
#define NET_TELNET_SPEC "N1:TELNET://10.4.0.1:4242/"
#define NET2_TELNET     "N2:TELNET://10.4.0.1:4242/"
#endif

/* NIST's daytime service sends one line and hangs up at once, so the line
 * and the close arrive together. A separate server for each, as NIST asks
 * for no more than one query every 4 seconds. */
#define NET_DAYTIME_TCP    "N1:TCP://time-a-g.nist.gov:13/"
#define NET_DAYTIME_TELNET "N1:TELNET://time-b-g.nist.gov:13/"
/* Every daytime line holds "UTC(NIST)", as ASCII bytes: cc65's is PETSCII on the C64 */
#define DAYTIME_MARK       "\x55\x54\x43\x28\x4e\x49\x53\x54\x29"

// Adam can't handle large web sites
//#define NET_DEVICESPEC  "N1:HTTPS://fujinet.online/"

#define HTTPBIN_ORG_ALT 3

#if HTTPBIN_ORG_ALT == 1
// If httpbin.org is down, run your own server with docker:
//  `docker run -p 8080:80 kennethreitz/httpbin`
#  define HTTPBIN_BASE "http://localhost:8080"
#elif HTTPBIN_ORG_ALT == 2
// If httpbin.org is down, run your own server with docker:
//  `docker run -p 7357:80 kennethreitz/httpbin`
#  define HTTPBIN_BASE "http://10.4.0.1:7357"
#elif HTTPBIN_ORG_ALT == 3
// Handles binary POST bodies correctly, unlike testotronic
#  define HTTPBIN_BASE "https://httpbin.dev"
#else
// httpbin.org is working
#  define HTTPBIN_BASE "HTTPS://httpbin.org"
#endif // HTTPBIN_ORG_ALT

#ifndef NET_DEVICESPEC
#define NET_DEVICESPEC  "N1:" HTTPBIN_BASE "/get"
#endif
#define NET_JSON_URL    "N1:" HTTPBIN_BASE "/json"
#define NET_EMPTY_URL   "N1:" HTTPBIN_BASE "/status/204"
#define NET_POST_URL    "N1:" HTTPBIN_BASE "/post"
#define NET_PUT_URL     "N1:" HTTPBIN_BASE "/put"
#define NET_DELETE_URL  "N1:" HTTPBIN_BASE "/delete"
#define NET_FORBIDDEN_URL "N1:" HTTPBIN_BASE "/status/403"

/* Complete HTTP framing arrives before a 23-second peer-close delay.
 * Keep the default reuse behavior: enabling keep-alive masks the regression. */
#define NET_HTTP_FIXED_HELD_URL "N1:http://wpt.live/common/echo.py?content=ABC&pipe=header%28Content-Length%2C3%29%7Ctrickle%283%3Ad23%29"
#define NET_HTTP_CHUNKED_HELD_URL "N1:http://wpt.live/common/echo.py?content=3%0D%0AABC%0D%0A0%0D%0A%0D%0A&pipe=header%28Transfer-Encoding%2Cchunked%29%7Ctrickle%2813%3Ad23%29"

/* NET_EMPTY_URL (defined per branch above) answers 204 with a zero-length
 * body. FNJSON::parse() conflates an empty body with malformed JSON and
 * FNHTML::parse() rejects an empty body outright, so it makes both parsers
 * fail -- see the parserr.c tests for fujinet-firmware #1640. */

#define TNFS_COPY_SOURCE_HOST "tnfs.fujinet.online"

/* A TNFS server the lock test can write to, running a tnfsd that answers
 * CHMOD (FujiNetWIFI/tnfsd#34). The test is skipped while this is unset. */
#define TNFS_RW_ROOT "N1:TNFS://ayce.fozztexx.com/test/"

/* fujinet-firmware#1836's repro: subdirectories 0-9 and A-Z, a LONG listing
 * well over 512 bytes. */
#define TNFS_BIG_DIR "N1:TNFS://apps.irata.online/Atari_8-bit/Games/Homesoft/"

/* Opens that fail for a known reason, to check what STATUS reports after. */
#define NET_MISSING_FILE   "N1:TNFS://" TNFS_COPY_SOURCE_HOST "/fnlibtest-never-created.txt"
#define NET_BAD_PROTOCOL   "N1:NOTAPROTOCOL://bad_host/"

/* Public WebDAV test server used by the network_fs tests. Anything created
 * at the root is removed by the server within 72 hours. Reached through
 * webdavserver.net rather than ajaxfilebrowser.com, whose certificate is
 * issued for webdavserver.net. */
#define WEBDAV_HOST     "www.webdavserver.net"
#define WEBDAV_ROOT     "N1:HTTPS://" WEBDAV_HOST "/"

/* Public anonymous FTP server -- the repro from fujinet-firmware PR #1652
 * (FozzTexx/fujinet-lib-experimental#117). Its root has been the same four
 * entries for years: a symlink to a directory (breakpoint -> users/breakpoint/),
 * robots.txt, the users directory, and welcome.msg. DIR_FORMAT_RAW appends '/'
 * to directories, so the names come back as breakpoint/ robots.txt users/
 * welcome.msg. Do not list users/ -- it is 658 entries and overruns
 * g.fs.listing. */
#define FTP_HOST        "ftp.untergrund.net"
#define FTP_ROOT        "N1:FTP://" FTP_HOST "/"
#define FTP_MISSING_DIR "N1:FTP://" FTP_HOST "/fnftptest-never-created/"

/* Deterministic byte stream (byte i == 'a' + i % 26) served with HTTP
 * Range support, for the seek/tell and read-count tests */
#define NET_RANGE_URL   "N1:" HTTPBIN_BASE "/range/1024"
#define RANGE_BYTE(i)   ((uint8_t) ('a' + ((i) % 26)))

/* Fixed HTML page (one h1: "Herman Melville - Moby-Dick") for HTML tests.
 * Its single <p> is several KB, so only query small elements. */
#define NET_HTML_URL    "N1:" HTTPBIN_BASE "/html"
/* Page of four small <a> anchors ("1".."4") for HTML query iteration */
#define NET_LINKS_URL   "N1:" HTTPBIN_BASE "/links/5/0"

/* UDP channel and destination for the set-destination smoke test
 * (no traffic is actually exchanged) */
#define NET_UDP_SPEC    "N1:UDP://:5004/"
#define NET_UDP_DEST    "N1:localhost:5005"

/* Listening TCP channel for accept / close-client */
#define NET_TCP_LISTEN  "N1:TCP://:6502/"

/* Calendar test fixture: a small .ics is PUT here over WebDAV, then read
 * back through the ICAL protocol (ICAL:// fetches over https). The name
 * is per-platform so parallel test runs cannot collide. */
#define CAL_ICS_NAME    "fncaltest-" PLATFORM ".ics"
#define CAL_ICS_PUT_URL  "N1:HTTPS://" WEBDAV_HOST "/" CAL_ICS_NAME
#define CAL_ICAL_BASE    "N1:ICAL://" WEBDAV_HOST "/" CAL_ICS_NAME
#define CAL_ICAL_URL     CAL_ICAL_BASE "/MONTH/2026-09"
#define CAL_EVENT_SUMMARY "FujiNet Test Event"

/* Directory listing formats, passed as the translation byte of a directory
 * open. RAW is exact filenames, no size column and no 8.3 crunching. */
#define DIR_FORMAT_LONG (0x80)
#define DIR_FORMAT_RAW  (0x83)

/* Constants that should be defined by fujinet-lib */
#define MAX_HOSTS 8
#if defined(BUILD_COCO) || defined(BUILD_PMD85)
#define MAX_DISKS 4
#elif defined(BUILD_APPLE2)
#define MAX_DISKS 10
#else
#define MAX_DISKS 8
#endif

/* Not defined by old fujinet-lib */
#if FNLIB_VERSION_MAJOR < 5
#define NETWORK_SUCCESS 0
#define NETWORK_ERROR_END_OF_FILE 136
#define NETWORK_ERROR_GENERAL 144
#define NETWORK_ERROR_FILE_NOT_FOUND 170
#define NETWORK_ERROR_NOT_CONNECTED 207
#define NETWORK_ERROR_COULD_NOT_PARSE_JSON 213
typedef uint8_t FN_ERR;
#endif

#endif /* CONSTANTS_H */
