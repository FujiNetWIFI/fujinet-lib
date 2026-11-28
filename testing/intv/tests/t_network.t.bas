' network: tests/network.c
    CONST MSG = $9300           ' 48 bytes: the echo message
    DIM m0, m1, m2, dp_a, dc_w, dc_l, pc_i

STR n2_https "N2:HTTPS://fujinet.online/"
STR n8_https "N8:HTTPS://fujinet.online/"

' MSG = the 3 prefix characters m0-m2 + " FujiNet integration test" + CRLF
' (the bus's native line ending); #x0 = its length
mk_msg: PROCEDURE
    POKE MSG, m0
    POKE MSG + 1, m1
    POKE MSG + 2, m2
    FOR #fn_i = 1 TO SL_echo_tail
        POKE MSG + 2 + #fn_i, PEEK(VARPTR str_echo_tail(0) + #fn_i - 1) AND 255
    NEXT #fn_i
    #x0 = 3 + SL_echo_tail
END

test_network_init: PROCEDURE
    SECTION "network_init"
    CHECK "init ok" @44, 1
END

' MSG (length #x0) to N1, read the echo, close
echo_check: PROCEDURE
    nw_unit = 1
    #fn_src = MSG : #cp_n = #x0 : GOSUB net_write
    CHECK "write ok" @60, fn_ok
    #cp_dst = NB_BUF : #cp_n = 64 : GOSUB buf_zero
    #cp_dst = NB_BUF : #cp_n = #x0 : GOSUB net_read
    CHECK "echo read" @68, #rd_n > 0
    IF #rd_n > 0 THEN
        #sa = NB_BUF : #sb = MSG : #sn = #x0 : GOSUB str_eq
        CHECK "echo matches" @74, t_same
    END IF
    GOSUB net_close
    CHECK "close tcp ok" @82, fn_ok
END

test_network_write: PROCEDURE
    SECTION "network_write"
    nw_mode = $0C : nw_trans = 2
    #fn_src = VARPTR str_tcp_spec(0) : #cs_len = SL_tcp_spec
    GOSUB net_open
    CHECK "open tcp rw" @100, fn_ok

    m0 = 87 : m1 = 82 : m2 = 84
    GOSUB mk_msg
    GOSUB echo_check

    nw_mode = $0C : nw_trans = 0
    #fn_src = VARPTR str_bogus_spec(0) : #cs_len = SL_bogus_spec
    GOSUB net_open
    CHECK "bogus open bad" @110, fn_ok = 0

    nw_unit = 1
    #fn_src = MSG : #cp_n = #x0 : GOSUB net_write
    CHECK "closed wr fails" @114, fn_ok = 0
END

' The 1200-byte pattern: 'A'+i%26, the last byte LF. gen_pat fills NB_BUF
' with #x3 bytes of it from offset #x2; chk_pat compares NB_BUF with it.
gen_pat: PROCEDURE
    FOR #fn_i = 0 TO #x3 - 1
        #x4 = #x2 + #fn_i
        IF #x4 = 1199 THEN
            POKE NB_BUF + #fn_i, 10
        ELSE
            POKE NB_BUF + #fn_i, 65 + (#x4 % 26)
        END IF
    NEXT #fn_i
END

chk_pat: PROCEDURE
    t_same = 1
    FOR #fn_i = 0 TO #x3 - 1
        #x4 = #x2 + #fn_i
        IF #x4 = 1199 THEN
            IF (PEEK(NB_BUF + #fn_i) AND 255) <> 10 THEN t_same = 0
        ELSE
            IF (PEEK(NB_BUF + #fn_i) AND 255) <> 65 + (#x4 % 26) THEN t_same = 0
        END IF
    NEXT #fn_i
END

test_network_write_large: PROCEDURE
    SECTION "write/read 1200"
    nw_mode = $0C : nw_trans = 0
    #fn_src = VARPTR str_tcp_spec(0) : #cs_len = SL_tcp_spec
    GOSUB net_open
    CHECK "open tcp rw" @147, fn_ok

    ' send in 240-byte pieces (the mailbox carries 256)
    #x6 = 1
    #x2 = 0
    WHILE #x2 < 1200
        #x3 = 240
        GOSUB gen_pat
        #fn_src = NB_BUF : #cp_n = 240 : GOSUB net_write
        IF fn_ok = 0 THEN #x6 = 0
        #x2 = #x2 + 240
    WEND
    CHECK "write 1200 ok" @149, #x6

    POKE NB_BUF + 256, $A5
    #x5 = 0                             ' total read
    #x6 = 1                             ' bytes match so far
    #x7 = 1                             ' keep going
    WHILE #x7
        IF #x5 >= 1200 THEN
            #x7 = 0
        ELSE
            #wd_lim = 500 : GOSUB wait_for_data
            IF #wd_bw = 0 THEN
                #x7 = 0
            ELSE
                #x3 = 1200 - #x5
                IF #x3 > 256 THEN #x3 = 256
                #cp_dst = NB_BUF : #cp_n = #x3 : GOSUB net_read
                IF #rd_n < 1 THEN
                    #x7 = 0
                ELSE
                    #x2 = #x5 : #x3 = #rd_n : GOSUB chk_pat
                    IF t_same = 0 THEN #x6 = 0
                    #x5 = #x5 + #rd_n
                END IF
            END IF
        END IF
    WEND
    CHECK "all 1200 back" @162, #x5 = 1200
    IF #x5 <> 1200 THEN #x6 = 0
    CHECK "echo matches" @163, #x6
    CHECK "end untouched" @164, (PEEK(NB_BUF + 256) AND 255) = $A5
    GOSUB net_close
    CHECK "close tcp ok" @167, fn_ok
END

test_network_set_eol: PROCEDURE
    SECTION "network_set_eol"
    nw_mode = $0C : nw_trans = 2
    #fn_src = VARPTR str_tcp_spec(0) : #cs_len = SL_tcp_spec
    GOSUB net_open
    CHECK "open tcp rw" @184, fn_ok

    nw_unit = 1
    #fn_src = VARPTR str_crlf(0) : #cs_len = 2
    GOSUB net_set_eol
    CHECK "set_eol ok" @191, fn_ok

    m0 = 69 : m1 = 79 : m2 = 76
    GOSUB mk_msg
    GOSUB echo_check

    nw_unit = 1
    #cs_len = 0
    GOSUB net_set_eol
    CHECK "set_eol reset" @201, fn_ok
END

test_network_unit: PROCEDURE
    SECTION "network_unit"
    #fn_src = VARPTR str_fn_https(0) : GOSUB net_unit_of
    CHECK "unit N1 is 1" @215, nw_unit = 1
    #fn_src = VARPTR str_n2_https(0) : GOSUB net_unit_of
    CHECK "unit N2 is 2" @216, nw_unit = 2
    #fn_src = VARPTR str_n8_https(0) : GOSUB net_unit_of
    CHECK "unit N8 is 8" @217, nw_unit = 8
END

test_network_status_after_failed_open: PROCEDURE
    SECTION "status failed open"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_missing_file(0) : #cs_len = SL_missing_file
    GOSUB net_open
    GOSUB net_status
    #x0 = 0
    IF fn_ok THEN
        IF nw_err = 170 THEN #x0 = 1
    END IF
    CHECK "missing is 170" @272, #x0
    GOSUB net_close

    #fn_src = VARPTR str_bad_proto(0) : #cs_len = SL_bad_proto
    GOSUB net_open
    GOSUB net_status
    #x0 = 0
    IF fn_ok THEN
        IF nw_err = 144 THEN #x0 = 1
    END IF
    CHECK "badproto is 144" @280, #x0
    GOSUB net_close
END

' Open a daytime server (spec #x4/#x5; pc_i 1 TCP, 2 TELNET): one line,
' then the peer hangs up.
peer_close_check: PROCEDURE
    nw_mode = $0C : nw_trans = 0
    #fn_src = #x4 : #cs_len = #x5
    GOSUB net_open
    CHECKL "TCP open ok|TELNET open ok" @405, pc_i, fn_ok

    #wd_lim = 500 : GOSUB wait_for_data
    #cp_dst = NB_BUF : #cp_n = 512 : GOSUB buf_zero
    #rd_n = 0
    IF #wd_bw > 0 THEN
        #cp_dst = NB_BUF : #cp_n = 511 : GOSUB net_read
    END IF
    #x0 = 0
    IF #rd_n > 0 THEN
        IF #rd_n < 511 THEN
            #sa = NB_BUF : #sb = VARPTR str_day_mark(0) : #sn = SL_day_mark
            GOSUB str_has
            #x0 = t_in
        END IF
    END IF
    CHECKL "TCP read line|TELNET read" @414, pc_i, #x0

    #x1 = 0
    wd_go = 1
    WHILE wd_go
        GOSUB net_status
        #x1 = #x1 + 1
        IF fn_ok = 0 THEN wd_go = 0
        IF nw_conn = 0 THEN
            IF #nw_avail = 0 THEN wd_go = 0
        END IF
        IF #x1 >= 50 THEN wd_go = 0
    WEND
    #x0 = 0
    IF fn_ok THEN
        IF nw_conn = 0 THEN
            IF #nw_avail = 0 THEN #x0 = 1
        END IF
    END IF
    CHECKL "TCP closes|TELNET closes" @426, pc_i, #x0
    GOSUB net_close
END

test_tcp_peer_close: PROCEDURE
    SECTION "TCP peer close"
    #x4 = VARPTR str_day_tcp(0) : #x5 = SL_day_tcp : pc_i = 1
    GOSUB peer_close_check
END

test_telnet_peer_close: PROCEDURE
    SECTION "TELNET peer close"
    #x4 = VARPTR str_day_tel(0) : #x5 = SL_day_tel : pc_i = 2
    GOSUB peer_close_check
END

test_network_open_no_n_prefix: PROCEDURE
    SECTION "open without N:"
    nw_unit = 2 : nw_mode = $0C : nw_trans = 2
    #fn_src = VARPTR str_tcp_nopfx(0) : #cs_len = SL_tcp_nopfx
    GOSUB net_open_u
    CHECK "open no N:" @458, fn_ok
    nw_unit = 2
    GOSUB net_close
END

test_network_status_after_close: PROCEDURE
    SECTION "status after close"
    m0 = 83 : m1 = 84 : m2 = 67
    GOSUB mk_msg
    nw_mode = $0C : nw_trans = 2
    #fn_src = VARPTR str_tcp_spec(0) : #cs_len = SL_tcp_spec
    GOSUB net_open
    #x1 = fn_ok
    IF #x1 THEN
        nw_unit = 1
        #fn_src = MSG : #cp_n = #x0 : GOSUB net_write
        #x1 = fn_ok
    END IF
    IF #x1 THEN
        nw_unit = 1
        GOSUB net_close
        #x1 = fn_ok
    END IF
    CHECK "open wr close" @485, #x1
    nw_unit = 1
    GOSUB net_status
    CHECK "status returns" @498, fn_ok
    GOSUB fuji_get_adapter_config
    CHECK "still answers" @503, fn_ok
END

' Two units open on the same echo server (specs #x4/#x5 and #x6/#x7);
' write to dc_w, data must show on it and not on dc_l.
dual_echo_check: PROCEDURE
    nw_unit = dc_w
    #fn_src = MSG : #cp_n = #x0 : GOSUB net_write
    #wd_lim = 500 : GOSUB wait_for_data
    CHECKI "N%d has bytes" 2 @316, dc_w, #wd_bw > 0

    nw_unit = dc_l
    #wd_lim = 5 : GOSUB wait_for_data
    CHECKI "N%d no bytes" 2 @322, dc_l, #wd_bw = 0

    #cp_dst = NB_BUF : #cp_n = 64 : GOSUB buf_zero
    nw_unit = dc_w
    #cp_dst = NB_BUF : #cp_n = #x0 : GOSUB net_read
    CHECKI "N%d echo read" 2 @328, dc_w, #rd_n > 0
    IF #rd_n > 0 THEN
        #sa = NB_BUF : #sb = MSG : #sn = #x0 : GOSUB str_eq
        CHECKI "N%d echo match" 2 @336, dc_w, t_same
    END IF
END

dual_echo_pair: PROCEDURE
    nw_mode = $0C : nw_trans = 2
    #fn_src = #x4 : #cs_len = #x5 : GOSUB net_open
    dp_a = fn_ok
    #fn_src = #x6 : #cs_len = #x7 : GOSUB net_open
    #x1 = 0
    IF dp_a THEN
        IF fn_ok THEN #x1 = 1
    END IF
    CHECK "dual open ok" @349, #x1

    m0 = 68 : m1 = 78 : m2 = 50
    GOSUB mk_msg
    dc_w = 2 : dc_l = 1
    GOSUB dual_echo_check
    m2 = 49
    GOSUB mk_msg
    dc_w = 1 : dc_l = 2
    GOSUB dual_echo_check

    nw_unit = 1 : GOSUB net_close
    nw_unit = 2 : GOSUB net_close
END

test_multiple_network_devices: PROCEDURE
    SECTION "multiple N: devices"
    #x4 = VARPTR str_tcp_spec(0) : #x5 = SL_tcp_spec
    #x6 = VARPTR str_tcp2_spec(0) : #x7 = SL_tcp2_spec
    GOSUB dual_echo_pair
END

test_multiple_telnet_devices: PROCEDURE
    SECTION "multiple TELNET"
    #x4 = VARPTR str_tel_spec(0) : #x5 = SL_tel_spec
    #x6 = VARPTR str_tel2_spec(0) : #x7 = SL_tel2_spec
    GOSUB dual_echo_pair
END

test_network: PROCEDURE
    GOSUB test_network_init : IF t_abort THEN RETURN
    GOSUB test_network_write : IF t_abort THEN RETURN
    GOSUB test_network_write_large : IF t_abort THEN RETURN
    GOSUB test_network_status_after_close : IF t_abort THEN RETURN
    GOSUB test_network_status_after_failed_open : IF t_abort THEN RETURN
    GOSUB test_network_open_no_n_prefix : IF t_abort THEN RETURN
    GOSUB test_network_set_eol : IF t_abort THEN RETURN
    GOSUB test_tcp_peer_close : IF t_abort THEN RETURN
    GOSUB test_telnet_peer_close : IF t_abort THEN RETURN
    GOSUB test_multiple_network_devices : IF t_abort THEN RETURN
    GOSUB test_multiple_telnet_devices : IF t_abort THEN RETURN
    GOSUB test_network_unit
END
