' netproto: tests/netproto.c
    DIM np_i, np_mode, np_pat, np_setup, np_go, np_eof, np_cnt, np_pok, np_nerr, np_ok
    DIM np_last

STR np_http "N1:https://httpbin.dev/range/1024"
STR np_ftpw "N1:FTP://ftp.untergrund.net/welcome.msg"
STR np_ftpr "N1:FTP://ftp.untergrund.net/robots.txt"
STR np_ftpd "N1:FTP://ftp.untergrund.net/"
STR np_davd "N1:HTTPS://www.webdavserver.net/fnproto/"
STR np_davf "N1:HTTPS://www.webdavserver.net/fnproto/fnprototest.txt"
STR np_tnfd "N1:TNFS://ayce.fozztexx.com/test/"
STR np_tnff "N1:TNFS://ayce.fozztexx.com/test/fnprototest.txt"
STR np_sdd "N1:SD:///fnproto/"
STR np_sdf "N1:SD:///fnproto/fnprototest.txt"
STR np_clip "N1:CLIPBOARD:///"
STR np_ical "N1:ICAL://www.webdavserver.net/fncaltest-intv.ics/MONTH/2026-09"
STR np_icsput "N1:HTTPS://www.webdavserver.net/fncaltest-intv.ics"
STR np_ics "BEGIN:VCALENDAR\r\nVERSION:2.0\r\nPRODID:-//FujiNet//fujinet-lib tests//EN\r\nBEGIN:VEVENT\r\nUID:fnlibtest-1@fujinet.online\r\nDTSTAMP:20260901T000000Z\r\nDTSTART:20260915T150000Z\r\nDTEND:20260915T160000Z\r\nSUMMARY:FujiNet Test Event\r\nEND:VEVENT\r\nEND:VCALENDAR\r\n"
STR np_prt "PRT... FujiNet integration test\r\n"

' Row np_i (1-12) of proto_rows: spec #x4/#x5, np_mode, size #x6, np_pat,
' np_setup (0 none, 1 dir, 2 file, 3 clipboard, 4 ics)
np_row: PROCEDURE
    np_mode = 4 : #x6 = 0 : np_pat = 0 : np_setup = 0
    IF np_i = 1 THEN
        #x4 = VARPTR str_np_http(0) : #x5 = SL_np_http : #x6 = 1024 : np_pat = 1
    ELSEIF np_i = 2 THEN
        #x4 = VARPTR str_np_ftpw(0) : #x5 = SL_np_ftpw : #x6 = 1329
    ELSEIF np_i = 3 THEN
        #x4 = VARPTR str_np_ftpr(0) : #x5 = SL_np_ftpr : #x6 = 26
    ELSEIF np_i = 4 THEN
        #x4 = VARPTR str_np_ftpd(0) : #x5 = SL_np_ftpd : np_mode = 6
    ELSEIF np_i = 5 THEN
        #x4 = VARPTR str_np_davd(0) : #x5 = SL_np_davd : np_mode = 6 : np_setup = 1
    ELSEIF np_i = 6 THEN
        #x4 = VARPTR str_np_davf(0) : #x5 = SL_np_davf : #x6 = 300 : np_pat = 1 : np_setup = 2
    ELSEIF np_i = 7 THEN
        #x4 = VARPTR str_np_tnfd(0) : #x5 = SL_np_tnfd : np_mode = 6
    ELSEIF np_i = 8 THEN
        #x4 = VARPTR str_np_tnff(0) : #x5 = SL_np_tnff : #x6 = 300 : np_pat = 1 : np_setup = 2
    ELSEIF np_i = 9 THEN
        #x4 = VARPTR str_np_sdd(0) : #x5 = SL_np_sdd : np_mode = 6 : np_setup = 1
    ELSEIF np_i = 10 THEN
        #x4 = VARPTR str_np_sdf(0) : #x5 = SL_np_sdf : #x6 = 300 : np_pat = 1 : np_setup = 2
    ELSEIF np_i = 11 THEN
        #x4 = VARPTR str_np_clip(0) : #x5 = SL_np_clip : #x6 = 300 : np_pat = 1 : np_setup = 3
    ELSE
        #x4 = VARPTR str_np_ical(0) : #x5 = SL_np_ical : np_setup = 4
    END IF
END

' put_file: 300 patterned bytes to the spec at #fn_src/#cs_len, mode 8 -> np_ok
np_put_file: PROCEDURE
    FOR #fn_i = 0 TO 299
        POKE NB_BUF + #fn_i, 97 + (#fn_i % 26)
    NEXT #fn_i
    np_ok = 0
    nw_mode = 8 : nw_trans = 0
    GOSUB net_open
    IF fn_ok = 0 THEN RETURN
    #fn_src = NB_BUF : #cp_n = 300 : GOSUB net_write
    IF fn_ok = 0 THEN
        GOSUB net_close
        RETURN
    END IF
    GOSUB net_close
    np_ok = fn_ok
END

np_put_ics: PROCEDURE
    np_ok = 0
    nw_mode = 8 : nw_trans = 0
    #fn_src = VARPTR str_np_icsput(0) : #cs_len = SL_np_icsput
    GOSUB net_open
    IF fn_ok = 0 THEN RETURN
    #fn_src = VARPTR str_np_ics(0) : #cp_n = SL_np_ics : GOSUB net_write
    IF fn_ok = 0 THEN
        GOSUB net_close
        RETURN
    END IF
    GOSUB net_close
    np_ok = fn_ok
END

np_setup_row: PROCEDURE
    GOSUB np_row
    IF np_setup = 1 THEN
        #fn_src = #x4 : #cs_len = #x5
        GOSUB net_unit_of
        mb_cmd = $2A
        GOSUB net_str0
    ELSEIF np_setup = 2 THEN
        #fn_src = #x4 : #cs_len = #x5 : GOSUB np_put_file
        CHECKL "DAV written|TNFS written|SD written" @188, np_last, np_ok
    ELSEIF np_setup = 3 THEN
        #fn_src = #x4 : #cs_len = #x5 : GOSUB np_put_file
        CHECK "CLIP written" @188, np_ok
    ELSEIF np_setup = 4 THEN
        GOSUB np_put_ics
        CHECK "ICAL feed ok" @191, np_ok
    END IF
END

np_cleanup_row: PROCEDURE
    GOSUB np_row
    IF np_setup = 1 THEN
        #fn_src = #x4 : #cs_len = #x5
        GOSUB net_unit_of
        mb_cmd = $2B
        GOSUB net_str0
        CHECKL "DAVdir removed|SDdir removed" @200, np_last, fn_ok
    ELSEIF np_setup = 2 THEN
        #fn_src = #x4 : #cs_len = #x5
        GOSUB net_unit_of
        mb_cmd = $21
        GOSUB net_str0
        CHECKL "DAV deleted|TNFS deleted|SD deleted" @203, np_last, fn_ok
    ELSEIF np_setup = 4 THEN
        #fn_src = VARPTR str_np_icsput(0) : #cs_len = SL_np_icsput
        GOSUB net_unit_of
        mb_cmd = $21
        GOSUB net_str0
        CHECK "ICAL feed gone" @206, fn_ok
    END IF
END

np_read_to_eof: PROCEDURE
    GOSUB np_row
    nw_trans = 0
    nw_mode = np_mode
    #fn_src = #x4 : #cs_len = #x5
    GOSUB net_open
    CHECKL "HTTP opens|FTP opens|FTPsm opens|FTPdir opens|DAVdir opens|DAV opens|TNFSdir opens|TNFS opens|SDdir opens|SD opens|CLIP opens|ICAL opens" @103, np_i, fn_ok

    #x2 = 0                             ' total
    #x3 = 0                             ' idle polls
    np_eof = 0 : np_pok = 1 : np_cnt = 1 : np_nerr = 0
    np_go = 1
    WHILE np_go
        IF #x2 >= 16384 THEN
            np_go = 0
        ELSEIF #x3 >= 1000 THEN
            np_go = 0
        ELSE
            GOSUB net_status
            np_nerr = nw_err
            IF fn_ok = 0 THEN
                np_go = 0
            ELSEIF #nw_avail = 0 THEN
                IF nw_conn = 0 THEN
                    np_eof = 1
                    np_go = 0
                ELSEIF nw_err = 136 THEN
                    np_eof = 1
                    np_go = 0
                ELSE
                    #x3 = #x3 + 1
                END IF
            ELSE
                #x7 = #nw_avail
                IF #x7 > 512 THEN #x7 = 512
                #cp_dst = NB_BUF : #cp_n = #x7 : GOSUB net_read
                IF #rd_n <> #x7 THEN
                    np_cnt = 0
                    np_go = 0
                ELSE
                    IF np_pat THEN
                        FOR #fn_i = 0 TO #x7 - 1
                            IF (PEEK(NB_BUF + #fn_i) AND 255) <> 97 + ((#x2 + #fn_i) % 26) THEN np_pok = 0
                        NEXT #fn_i
                    END IF
                    #x2 = #x2 + #x7
                    #x3 = 0
                END IF
            END IF
        END IF
    WEND

    CHECKL "HTTP count|FTP count|FTPsm count|FTPdir count|DAVdir count|DAV count|TNFSdir count|TNFS count|SDdir count|SD count|CLIP count|ICAL count" @136, np_i, np_cnt
    CHECKL "HTTP end|FTP end|FTPsm end|FTPdir end|DAVdir end|DAV end|TNFSdir end|TNFS end|SDdir end|SD end|CLIP end|ICAL end" @137, np_i, np_eof
    CHECKL "HTTP EOF|FTP EOF|FTPsm EOF|FTPdir EOF|DAVdir EOF|DAV EOF|TNFSdir EOF|TNFS EOF|SDdir EOF|SD EOF|CLIP EOF|ICAL EOF" @138, np_i, np_nerr = 136
    np_ok = 0
    IF #x6 <> 0 THEN
        IF #x2 = #x6 THEN np_ok = 1
    ELSE
        IF #x2 > 0 THEN np_ok = 1
    END IF
    CHECKL "HTTP size|FTP size|FTPsm size|FTPdir data|DAVdir data|DAV size|TNFSdir data|TNFS size|SDdir data|SD size|CLIP size|ICAL data" @140, np_i, np_ok
    IF np_pat THEN
        CHECKL "HTTP bytes|FTP bytes|FTPsm bytes|FTPdir bytes|DAVdir bytes|DAV bytes|TNFSdir bytes|TNFS bytes|SDdir bytes|SD bytes|CLIP bytes|ICAL bytes" @144, np_i, np_pok
    END IF

    #cp_dst = NB_BUF : #cp_n = 16 : GOSUB net_read_nb
    CHECKL "HTTP past EOF|FTP past EOF|FTPsm past EOF|FTPdir past EOF|DAVdir past EOF|DAV past EOF|TNFSd past EOF|TNFS past EOF|SDdir past EOF|SD past EOF|CLIP past EOF|ICAL past EOF" @147, np_i, #rd_n = 0
    GOSUB net_close
    CHECKL "HTTP closes|FTP closes|FTPsm closes|FTPdir closes|DAVdir closes|DAV closes|TNFSdir closes|TNFS closes|SDdir closes|SD closes|CLIP closes|ICAL closes" @150, np_i, fn_ok
END

test_netproto_read_count: PROCEDURE
    SECTION "read honors count"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_np_http(0) : #cs_len = SL_np_http
    GOSUB net_open
    CHECK "open range" @220, fn_ok
    #cp_dst = NB_BUF : #cp_n = 64 : GOSUB buf_zero
    #cp_dst = NB_BUF : #cp_n = 64 : GOSUB net_read
    CHECK "reads 64" @223, #rd_n = 64
    #x0 = 1
    FOR #fn_i = 0 TO 63
        IF (PEEK(NB_BUF + #fn_i) AND 255) <> 97 + (#fn_i % 26) THEN #x0 = 0
    NEXT #fn_i
    CHECK "64 bytes match" @226, #x0
    GOSUB net_close
    CHECK "close range" @228, fn_ok
END

test_netproto_read_to_eof: PROCEDURE
    SECTION "protocols to EOF"
    FOR np_i = 1 TO 12
        np_last = 0
        IF np_i = 6 THEN np_last = 1
        IF np_i = 8 THEN np_last = 2
        IF np_i = 10 THEN np_last = 3
        GOSUB np_setup_row
        IF t_abort THEN RETURN
    NEXT np_i
    FOR np_i = 1 TO 12
        GOSUB np_read_to_eof
        IF t_abort THEN RETURN
    NEXT np_i
    FOR np_i = 12 TO 1 STEP -1
        np_last = 0
        IF np_i = 5 THEN np_last = 1
        IF np_i = 9 THEN np_last = 2
        IF np_i = 6 THEN np_last = 1
        IF np_i = 8 THEN np_last = 2
        IF np_i = 10 THEN np_last = 3
        GOSUB np_cleanup_row
        IF t_abort THEN RETURN
    NEXT np_i
END

test_netproto_read_partial: PROCEDURE
    SECTION "read partial"
    nw_mode = $0C : nw_trans = 2
    #fn_src = VARPTR str_tcp_spec(0) : #cs_len = SL_tcp_spec
    GOSUB net_open
    nw_unit = 1
    #fn_src = VARPTR str_np_prt(0) : #cp_n = SL_np_prt : GOSUB net_write
    #cp_dst = NB_BUF : #cp_n = 64 : GOSUB buf_zero
    #x1 = SL_np_prt / 2
    #cp_dst = NB_BUF : #cp_n = #x1 : GOSUB net_read
    CHECK "partial length" @271, #rd_n = #x1
    #x2 = SL_np_prt - #x1
    #cp_dst = NB_BUF + #x1 : #cp_n = #x2 : GOSUB net_read
    CHECK "rest length" @273, #rd_n = #x2
    #sa = NB_BUF : #sb = VARPTR str_np_prt(0) : #sn = SL_np_prt : GOSUB str_eq
    CHECK "all data read" @276, t_same
    GOSUB net_close
END

test_netproto: PROCEDURE
    GOSUB test_netproto_read_count : IF t_abort THEN RETURN
    GOSUB test_netproto_read_to_eof : IF t_abort THEN RETURN
    GOSUB test_netproto_read_partial
END
