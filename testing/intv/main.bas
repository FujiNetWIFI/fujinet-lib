' main.bas -- fujinet-lib integration tests for the Intellivision.
' One cart, one menu entry per fujinet-lib test program.
    OPTION MAP 4
    INCLUDE "build/config.bas"      ' CONST AUTORUN (0 = menu)
    GOTO start

    INCLUDE "harness.bas"
    INCLUDE "fnlib.bas"
    INCLUDE "fnnet.bas"

' Test programs. Segments are the ROM regions of OPTION MAP 4; the tests
' fill them in turn and the check names go last.
    SEGMENT 3
    INCLUDE "build/t_consts.bas"

    BANK 0
    INCLUDE "build/t_appkey.bas"
    BANK 1
    INCLUDE "build/t_clock.bas"
    BANK 2
    INCLUDE "build/t_devlist.bas"
    BANK 3
    INCLUDE "build/t_wifi.bas"
    BANK 4
    INCLUDE "build/t_fujidev.bas"
    BANK 5
    INCLUDE "build/t_network.bas"
    BANK 6
    INCLUDE "build/t_net5plus.bas"
    BANK 7
    INCLUDE "build/t_parserr.bas"
    BANK 8
    INCLUDE "build/t_html.bas"
    BANK 9
    INCLUDE "build/t_clipbord.bas"
    BANK 10
    INCLUDE "build/t_netproto.bas"
    BANK 11
    INCLUDE "build/t_qrcode.bas"
    BANK 12
    INCLUDE "build/t_disk.bas"
    BANK 13
    INCLUDE "build/t_netfile.bas"

    SEGMENT 2
    INCLUDE "build/names.bas"

    SEGMENT 0

    DIM prog, page, key_now

' ---- programs: the order of fujinet-lib's testing/Makefile PRODUCTS ----
run_appkey: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "APPKEY"
    GOSUB fn_wait_mailbox
    BANK SELECT 0
    GOSUB test_appkey
    GOSUB end_testing
END

run_clock: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "CLOCK"
    GOSUB fn_wait_mailbox
    BANK SELECT 1
    GOSUB test_clock
    GOSUB end_testing
END

run_devlist: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "DEVLIST"
    GOSUB fn_wait_mailbox
    BANK SELECT 2
    GOSUB test_devlist
    GOSUB end_testing
END

run_wifi: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "WIFI"
    GOSUB fn_wait_mailbox
    BANK SELECT 3
    GOSUB test_wifi
    GOSUB end_testing
END

run_fujidev: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "FUJIDEV"
    GOSUB fn_wait_mailbox
    BANK SELECT 4
    GOSUB test_fujidev
    GOSUB end_testing
END

run_network: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "NETWORK"
    GOSUB fn_wait_mailbox
    BANK SELECT 5
    GOSUB test_network
    GOSUB end_testing
END

run_net5plus: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "NET5PLUS"
    GOSUB fn_wait_mailbox
    BANK SELECT 6
    GOSUB test_net5plus
    GOSUB end_testing
END

run_parserr: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "PARSERR"
    GOSUB fn_wait_mailbox
    BANK SELECT 7
    GOSUB test_parserr
    GOSUB end_testing
END

run_html: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "HTML"
    GOSUB fn_wait_mailbox
    BANK SELECT 8
    GOSUB test_html
    GOSUB end_testing
END

run_clipbord: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "CLIPBORD"
    GOSUB fn_wait_mailbox
    BANK SELECT 9
    GOSUB test_clipbord
    GOSUB end_testing
END

run_netproto: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "NETPROTO"
    GOSUB fn_wait_mailbox
    BANK SELECT 10
    GOSUB test_netproto
    GOSUB end_testing
END

run_qrcode: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "QRCODE"
    GOSUB fn_wait_mailbox
    BANK SELECT 11
    GOSUB test_qrcode
    GOSUB end_testing
END

run_disk: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "DISK"
    GOSUB fn_wait_mailbox
    BANK SELECT 12
    GOSUB test_disk
    GOSUB end_testing
END

run_netfile: PROCEDURE
    GOSUB begin_testing
    PRINT AT 0 COLOR C_WHITE, "NETFILE"
    GOSUB fn_wait_mailbox
    BANK SELECT 13
    GOSUB test_netfile
    GOSUB end_testing
END

run_prog: PROCEDURE
    IF prog = 1 THEN
        GOSUB run_fujidev
    ELSEIF prog = 2 THEN
        GOSUB run_appkey
    ELSEIF prog = 3 THEN
        GOSUB run_clock
    ELSEIF prog = 4 THEN
        GOSUB run_devlist
    ELSEIF prog = 7 THEN
        GOSUB run_net5plus
    ELSEIF prog = 6 THEN
        GOSUB run_netfile
    ELSEIF prog = 5 THEN
        GOSUB run_disk
    ELSEIF prog = 10 THEN
        GOSUB run_qrcode
    ELSEIF prog = 13 THEN
        GOSUB run_netproto
    ELSEIF prog = 12 THEN
        GOSUB run_clipbord
    ELSEIF prog = 11 THEN
        GOSUB run_html
    ELSEIF prog = 9 THEN
        GOSUB run_parserr
    ELSEIF prog = 8 THEN
        GOSUB run_network
    ELSEIF prog = 14 THEN
        GOSUB run_wifi
    END IF
END

menu: PROCEDURE
    CLS
    PRINT AT 0 COLOR C_WHITE, "FUJINET LIB TESTS"
    IF page = 0 THEN
        PRINT AT 20 COLOR C_GREEN, "1 FUJIDEV 6 NETFILE"
        PRINT AT 40 COLOR C_GREEN, "2 APPKEY  7 NET5PLUS"
        PRINT AT 60 COLOR C_GREEN, "3 CLOCK   8 NETWORK"
        PRINT AT 80 COLOR C_GREEN, "4 DEVLIST  9 PARSERR"
        PRINT AT 100 COLOR C_GREEN, "5 DISK    0 QRCODE"
    ELSE
        PRINT AT 20 COLOR C_GREEN, "1 HTML"
        PRINT AT 40 COLOR C_GREEN, "2 CLIPBORD"
        PRINT AT 60 COLOR C_GREEN, "3 NETPROTO"
        PRINT AT 80 COLOR C_GREEN, "4 WIFI"
    END IF
    PRINT AT 200 COLOR C_YELLOW, "ENTER: OTHER PAGE"
END

start:
    MODE 0, 0, 0, 0, 0
    BORDER 0
    WAIT
    IF AUTORUN > 0 THEN
        prog = AUTORUN
        GOSUB run_prog
        GOTO idle
    END IF
    page = 0
main_menu:
    GOSUB menu
    WHILE CONT.KEY = 12
        WAIT
    WEND
    key_now = CONT.KEY
    WHILE CONT.KEY <> 12
        WAIT
    WEND
    IF key_now = 11 THEN
        page = 1 - page
        GOTO main_menu
    END IF
    IF key_now > 9 THEN GOTO main_menu
    IF page = 0 THEN
        IF key_now = 0 THEN
            prog = 10
        ELSE
            prog = key_now
        END IF
    ELSE
        IF key_now < 1 THEN GOTO main_menu
        IF key_now > 4 THEN GOTO main_menu
        prog = key_now + 10
    END IF
    GOSUB run_prog
    GOTO main_menu

idle:
    WAIT
    GOTO idle
