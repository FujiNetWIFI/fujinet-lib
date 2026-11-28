' net5plus: tests/net5plus.c
    DIM n5_i, n5_ok, n5_c

STR sock_listen "N1:TCP://:5432"
STR sock_client "N2:TCP://localhost:5432"
STR sock_cli2 "N3:TCP://localhost:5432"
STR udp_client "N2:UDP://localhost:5004"
STR tcp_listen "N1:TCP://:6502/"
STR udp_spec "N1:UDP://:5004/"
STR udp_dest "N1:localhost:5005"
STR n4_spec "N4:"
STR n1_spec "N1:"
STR pat16 "0123456789ABCDEF"
STR wxyz "wxyz"
STR fuji4 "FUJI"
STR user_s "fujinet-test-user"
STR pass_s "fujinet-test-pass"

' Wait up to #wd_lim polls for nw_unit's connection to drop -> n5_ok
wait_disconnected: PROCEDURE
    n5_ok = 1
    #wd_r = 0
    wd_go = 1
    WHILE wd_go
        GOSUB net_status
        IF fn_ok = 0 THEN
            n5_ok = 0
            wd_go = 0
        ELSE
            IF nw_conn = 0 THEN wd_go = 0
        END IF
        #wd_r = #wd_r + 1
        IF #wd_r >= 500 THEN wd_go = 0
    WEND
    IF fn_ok THEN
        IF nw_conn <> 0 THEN n5_ok = 0
    END IF
END

' Open the listener on N1 and the client on N2, wait for the connection and
' accept it. n5_ok = every step worked.
open_pair: PROCEDURE
    nw_mode = $0C : nw_trans = 0
    #fn_src = VARPTR str_sock_listen(0) : #cs_len = SL_sock_listen
    GOSUB net_open
    #x5 = fn_ok
    #fn_src = VARPTR str_sock_client(0) : #cs_len = SL_sock_client
    GOSUB net_open
    #x6 = fn_ok
END

' Poll the listener (N1) up to 10 times for a connection (status byte 1)
listener_ready: PROCEDURE
    nw_unit = 1
    n5_i = 0
    wd_go = 1
    WHILE wd_go
        GOSUB net_status
        n5_i = n5_i + 1
        IF fn_ok = 0 THEN wd_go = 0
        IF nw_conn = 1 THEN wd_go = 0
        IF n5_i >= 10 THEN wd_go = 0
    WEND
END

' relay_byte(n5_c): the client (N2) writes it, the listener (N1) must read it
relay_byte: PROCEDURE
    n5_ok = 0
    POKE NB_SPEC, n5_c
    nw_unit = 2
    #fn_src = NB_SPEC : #cp_n = 1 : GOSUB net_write
    IF fn_ok = 0 THEN RETURN
    nw_unit = 1
    #wd_lim = 500 : GOSUB wait_for_data
    IF #wd_bw = 0 THEN RETURN
    POKE NB_BUF, 0
    #cp_dst = NB_BUF : #cp_n = 1 : GOSUB net_read
    IF #rd_n = 1 THEN
        IF (PEEK(NB_BUF) AND 255) = n5_c THEN n5_ok = 1
    END IF
END

test_net2_seek_tell: PROCEDURE
    SECTION "seek/tell"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_range_url(0) : #cs_len = SL_range_url
    GOSUB net_open
    CHECK "open range" @56, fn_ok

    #cp_dst = NB_BUF : #cp_n = 16 : GOSUB net_read
    CHECK "read 16" @59, #rd_n = 16
    n5_ok = 1
    FOR n5_i = 0 TO 15
        IF (PEEK(NB_BUF + n5_i) AND 255) <> 97 + n5_i THEN n5_ok = 0
    NEXT n5_i
    CHECK "pattern ok" @64, n5_ok

    GOSUB net_tell
    CHECK "tell ok" @68, fn_ok
    #x0 = 0
    IF #nw_pos = 16 THEN
        IF #nw_poshi = 0 THEN #x0 = 1
    END IF
    CHECK "tell is 16" @70, #x0

    #nw_pos = 256 : #nw_poshi = 0
    GOSUB net_seek
    CHECK "seek 256 ok" @73, fn_ok
    GOSUB net_tell
    CHECK "tell after ok" @77, fn_ok
    #x0 = 0
    IF #nw_pos = 256 THEN
        IF #nw_poshi = 0 THEN #x0 = 1
    END IF
    CHECK "tell is 256" @78, #x0

    #cp_dst = NB_BUF : #cp_n = 4 : GOSUB net_read
    CHECK "read 4" @81, #rd_n = 4
    #sa = NB_BUF : #sb = VARPTR str_wxyz(0) : #sn = 4 : GOSUB str_eq
    CHECK "from offset" @83, t_same
    GOSUB net_close
    CHECK "close range" @87, fn_ok
END

test_net2_seek_errors: PROCEDURE
    SECTION "seek errors"
    nw_unit = 4 : #nw_pos = 0 : #nw_poshi = 0
    GOSUB net_seek
    CHECK "seek unopen bad" @104, fn_ok = 0

    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_json_url(0) : #cs_len = SL_json_url
    GOSUB net_open
    CHECK "open json" @108, fn_ok
    GOSUB net_json_parse
    CHECK "parse json" @110, fn_ok
    #nw_pos = 0 : #nw_poshi = 0
    GOSUB net_seek
    CHECK "json seek bad" @112, fn_ok = 0
    GOSUB net_close
    CHECK "close json" @114, fn_ok
END

test_net2_bus_read_unopened: PROCEDURE
    SECTION "read unopened unit"
    nw_unit = 4
    #cp_dst = NB_BUF : #cp_n = 16 : GOSUB net_bus_read
    CHECK "failed read 0" @130, fn_ok = 0
END

test_net2_set_translation: PROCEDURE
    SECTION "set translation"
    nw_unit = 1
    mb_cmd = NETCMD_TRANS
    nw_a = 2 : GOSUB net_a12
    CHECK "trans LF ok" @148, fn_ok
    nw_a = $FF : GOSUB net_a12
    CHECK "trans FF ok" @151, fn_ok
    nw_a = 0 : GOSUB net_a12
    CHECK "trans none ok" @154, fn_ok
END

test_net2_login_smoke: PROCEDURE
    SECTION "username/password"
    nw_unit = 1
    mb_cmd = NETCMD_USERNAME
    #fn_src = VARPTR str_user_s(0) : #cs_len = SL_user_s : GOSUB net_str0
    CHECK "set user ok" @172, fn_ok
    mb_cmd = NETCMD_PASSWORD
    #fn_src = VARPTR str_pass_s(0) : #cs_len = SL_pass_s : GOSUB net_str0
    CHECK "set pass ok" @175, fn_ok
    mb_cmd = NETCMD_USERNAME
    #cs_len = 0 : GOSUB net_str0
    CHECK "clear user ok" @179, fn_ok
    mb_cmd = NETCMD_PASSWORD
    #cs_len = 0 : GOSUB net_str0
    CHECK "clear pass ok" @181, fn_ok
END

test_net2_interrupt_rate: PROCEDURE
    SECTION "interrupt rate"
    nw_unit = 1
    mb_cmd = NETCMD_INT_RATE
    nw_a = 50 : GOSUB net_a12
    CHECK "rate 50 ok" @197, fn_ok
    nw_a = 100 : GOSUB net_a12
    CHECK "rate 100 ok" @200, fn_ok
END

test_net2_close_client: PROCEDURE
    SECTION "close-client"
    nw_mode = $0C : nw_trans = 0
    #fn_src = VARPTR str_tcp_listen(0) : #cs_len = SL_tcp_listen
    GOSUB net_open
    CHECK "open listener" @228, fn_ok
    nw_unit = 1
    mb_cmd = NETCMD_CLOSE_CLI
    GOSUB net_cmd
    CHECK "no client ok" @233, fn_ok
    GOSUB net_close
    CHECK "close listener" @236, fn_ok

    GOSUB open_pair
    CHECK "open live lstn" @239, #x5
    CHECK "client connects" @241, #x6
    GOSUB listener_ready
    nw_unit = 1
    mb_cmd = NETCMD_ACCEPT
    GOSUB net_cmd
    CHECK "accept client" @248, fn_ok

    mb_cmd = NETCMD_CLOSE_CLI
    GOSUB net_cmd
    CHECK "close client ok" @251, fn_ok
    nw_unit = 1 : GOSUB wait_disconnected
    CHECK "listener drops" @252, n5_ok
    nw_unit = 2 : GOSUB wait_disconnected
    CHECK "client sees cls" @253, n5_ok

    nw_unit = 2
    mb_cmd = NETCMD_CLOSE_CLI
    GOSUB net_cmd
    CHECK "cc non-lstn bad" @256, fn_ok = 0
    nw_unit = 2 : GOSUB net_close
    nw_unit = 1 : GOSUB net_close
END

test_net2_udp_set_destination: PROCEDURE
    SECTION "UDP destination"
    nw_mode = $0C : nw_trans = 0
    #fn_src = VARPTR str_udp_spec(0) : #cs_len = SL_udp_spec
    GOSUB net_open
    CHECK "open udp" @275, fn_ok
    nw_unit = 1
    #fn_src = VARPTR str_udp_dest(0) : #cs_len = SL_udp_dest
    mb_cmd = NETCMD_DEST
    mb_dev = NET_DEV0 + nw_unit
    mb_nparam = 0
    #cp_n = #cs_len : GOSUB tx_put
    GOSUB fn_transact
    CHECK "set dest ok" @278, fn_ok
    GOSUB net_close
    CHECK "close udp" @281, fn_ok
END

test_net2_udp_get_remote: PROCEDURE
    SECTION "UDP remote"
    nw_mode = $0C : nw_trans = 0
    #fn_src = VARPTR str_udp_spec(0) : #cs_len = SL_udp_spec
    GOSUB net_open
    CHECK "open udp" @299, fn_ok
    nw_unit = 1
    mb_cmd = NETCMD_REMOTE
    GOSUB net_cmd
    CHECK "get remote ok" @303, fn_ok
    GOSUB net_close
    CHECK "close udp" @307, fn_ok
END

test_net2_udp_round_trip: PROCEDURE
    SECTION "UDP round trip"
    nw_mode = $0C : nw_trans = 0
    #fn_src = VARPTR str_udp_spec(0) : #cs_len = SL_udp_spec
    GOSUB net_open
    CHECK "open udp lstn" @321, fn_ok
    #fn_src = VARPTR str_udp_client(0) : #cs_len = SL_udp_client
    GOSUB net_open
    CHECK "open udp client" @323, fn_ok

    nw_unit = 2
    #fn_src = VARPTR str_fuji4(0) : #cp_n = 4 : GOSUB net_write
    CHECK "client sends" @326, fn_ok

    nw_unit = 1
    #x0 = 0
    #wd_r = 0
    wd_go = 1
    WHILE wd_go
        GOSUB net_status
        IF fn_ok = 0 THEN wd_go = 0
        IF #nw_avail > 0 THEN wd_go = 0
        #wd_r = #wd_r + 1
        IF #wd_r >= 500 THEN wd_go = 0
    WEND
    IF fn_ok THEN
        IF #nw_avail = 4 THEN #x0 = 1
    END IF
    CHECK "status has 4" @336, #x0

    #cp_dst = NB_BUF : #cp_n = 4 : GOSUB buf_zero
    #cp_dst = NB_BUF : #cp_n = 4 : GOSUB net_read
    #x0 = 0
    IF #rd_n = 4 THEN
        #sa = NB_BUF : #sb = VARPTR str_fuji4(0) : #sn = 4 : GOSUB str_eq
        #x0 = t_same
    END IF
    CHECK "read datagram" @340, #x0
    nw_unit = 2 : GOSUB net_close
    nw_unit = 1 : GOSUB net_close
END

test_net2_channel_mode: PROCEDURE
    SECTION "channel mode"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_json_url(0) : #cs_len = SL_json_url
    GOSUB net_open
    CHECK "open json" @359, fn_ok
    nw_a = 1 : GOSUB net_set_parser
    CHECK "to JSON mode" @362, fn_ok
    nw_a = 0 : GOSUB net_set_parser
    CHECK "back to PROTO" @365, fn_ok
    #cp_dst = NB_BUF : #cp_n = 64 : GOSUB net_read
    CHECK "body readable" @369, #rd_n > 0
    GOSUB net_close
    CHECK "close json" @372, fn_ok
END

test_net2_accept: PROCEDURE
    SECTION "network_accept"
    nw_mode = $0C : nw_trans = 0
    GOSUB open_pair
    CHECK "open listen" @413, #x5
    CHECK "open client" @415, #x6
    GOSUB listener_ready
    #x0 = 0
    IF fn_ok THEN
        IF nw_conn = 1 THEN
            IF n5_i < 10 THEN #x0 = 1
        END IF
    END IF
    CHECK "listener conn" @426, #x0
    nw_unit = 1
    mb_cmd = NETCMD_ACCEPT
    GOSUB net_cmd
    CHECK "accept ok" @429, fn_ok
    n5_c = 65 : GOSUB relay_byte
    CHECK "byte reaches" @430, n5_ok

    nw_mode = $0C : nw_trans = 0
    #fn_src = VARPTR str_sock_cli2(0) : #cs_len = SL_sock_cli2
    GOSUB net_open
    CHECK "second client" @437, fn_ok
    nw_unit = 1
    mb_cmd = NETCMD_ACCEPT
    GOSUB net_cmd
    CHECK "2nd accept bad" @439, fn_ok = 0
    GOSUB net_status
    #x0 = 0
    IF fn_ok THEN
        IF nw_err = 205 THEN #x0 = 1
    END IF
    CHECK "in progress" @443, #x0
    n5_c = 66 : GOSUB relay_byte
    CHECK "session lives" @444, n5_ok
    nw_unit = 3 : GOSUB net_close
    nw_unit = 2 : GOSUB net_close
    nw_unit = 1 : GOSUB net_close
END

' A clock second counter, the way the C test waits: two SIMPLE_BINARY
' seconds ticks
sec_ticks2: PROCEDURE
    cf_fmt = 0 : cf_alt = 0
    GOSUB clock_get_time
    #x7 = PEEK(CLK_BUF + 6) AND 255
    n5_c = #x7
    n5_i = 0
    WHILE n5_i < 2
        GOSUB clock_get_time
        IF (PEEK(CLK_BUF + 6) AND 255) <> n5_c THEN
            #x7 = PEEK(CLK_BUF + 6) AND 255
            n5_c = #x7
            n5_i = n5_i + 1
        END IF
    WEND
END

test_net2_short_read: PROCEDURE
    SECTION "short TCP read"
    GOSUB open_pair
    CHECK "open listening" @472, #x5
    CHECK "open client" @474, #x6
    GOSUB listener_ready
    nw_unit = 1
    mb_cmd = NETCMD_ACCEPT
    GOSUB net_cmd
    CHECK "accept ok" @481, fn_ok
    nw_unit = 2
    #fn_src = VARPTR str_pat16(0) : #cp_n = 16 : GOSUB net_write
    CHECK "client wr 16" @484, fn_ok

    GOSUB sec_ticks2
    FOR n5_i = 0 TO 63
        POKE NB_BUF + n5_i, $A5
    NEXT n5_i
    nw_unit = 1
    #cp_dst = NB_BUF : #cp_n = 64 : GOSUB net_bus_read
    #x0 = 0
    IF fn_ok THEN
        IF #fn_t >= 16 THEN #x0 = 1
    END IF
    CHECK "read gets data" @500, #x0
    #sa = NB_BUF : #sb = VARPTR str_pat16(0) : #sn = 16 : GOSUB str_eq
    CHECK "16 bytes intact" @501, t_same
    nw_unit = 2 : GOSUB net_close
    nw_unit = 1 : GOSUB net_close
END

test_net5plus: PROCEDURE
    GOSUB test_net2_accept : IF t_abort THEN RETURN
    GOSUB test_net2_short_read : IF t_abort THEN RETURN
    GOSUB test_net2_seek_tell : IF t_abort THEN RETURN
    GOSUB test_net2_seek_errors : IF t_abort THEN RETURN
    GOSUB test_net2_bus_read_unopened : IF t_abort THEN RETURN
    GOSUB test_net2_set_translation : IF t_abort THEN RETURN
    GOSUB test_net2_login_smoke : IF t_abort THEN RETURN
    GOSUB test_net2_interrupt_rate : IF t_abort THEN RETURN
    GOSUB test_net2_close_client : IF t_abort THEN RETURN
    GOSUB test_net2_udp_set_destination : IF t_abort THEN RETURN
    GOSUB test_net2_udp_get_remote : IF t_abort THEN RETURN
    GOSUB test_net2_udp_round_trip : IF t_abort THEN RETURN
    GOSUB test_net2_channel_mode
END
