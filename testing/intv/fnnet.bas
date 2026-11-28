' fnnet.bas -- network device ($71-$78) calls, shaped like fujinet-lib's
' common/network_*.c on a variable-length-packet bus. The device spec is
' at #fn_src, its length in #cs_len; the unit comes from the spec ("N2:").

    CONST NET_DEV0 = $70        ' + unit
    CONST NETCMD_OPEN      = $4F
    CONST NETCMD_CLOSE     = $43
    CONST NETCMD_STATUS    = $53
    CONST NETCMD_READ      = $52
    CONST NETCMD_WRITE     = $57
    CONST NETCMD_CHDIR     = $2C
    CONST NETCMD_GETCWD    = $30

    CONST NB_SPEC = $9000       ' 96 bytes: a spec built at run time
    CONST NB_BUF  = $9060       ' 512 bytes: network data
    CONST NB_PATH = $9260       ' 128 bytes: current directory reply

    DIM nw_unit, nw_mode, nw_trans
    DIM #nw_avail, nw_conn, nw_err
    DIM #rd_n, rd_bad
    DIM #nw_left, #nw_chunk, #nw_total

' nw_unit from the spec at #fn_src: "N:" is 1, "Nk:" is k, else 1.
net_unit_of: PROCEDURE
    nw_unit = 1
    IF (PEEK(#fn_src + 1) AND 255) <> 58 THEN
        IF (PEEK(#fn_src + 2) AND 255) = 58 THEN
            #fn_t = PEEK(#fn_src + 1) AND 255
            nw_unit = #fn_t - 48
        END IF
    END IF
END

' Payload-less command to the unit: mb_cmd set by the caller.
net_cmd: PROCEDURE
    mb_dev = NET_DEV0 + nw_unit
    mb_nparam = 0
    #fn_txlen = 0
    GOSUB fn_transact
END

' network_open(spec, nw_mode, nw_trans) -> fn_ok
net_open: PROCEDURE
    GOSUB net_unit_of
    GOSUB net_open_u
END

' The same with nw_unit already set (a spec that has no "Nk:" prefix)
net_open_u: PROCEDURE
    mb_dev = NET_DEV0 + nw_unit
    mb_cmd = NETCMD_OPEN
    mb_nparam = 2
    pm_i = 0 : pm_size = 1 : #pm_val = nw_mode : GOSUB fn_param
    pm_i = 1 : pm_size = 1 : #pm_val = nw_trans : GOSUB fn_param
    #cp_n = #cs_len
    GOSUB tx_put
    GOSUB fn_transact
END

' network_close(unit) -> fn_ok
net_close: PROCEDURE
    mb_cmd = NETCMD_CLOSE
    GOSUB net_cmd
END

' network_unit_status(unit) -> fn_ok, #nw_avail, nw_conn, nw_err
net_status: PROCEDURE
    mb_cmd = NETCMD_STATUS
    GOSUB net_cmd
    #nw_avail = 0 : nw_conn = 0 : nw_err = 0
    IF fn_ok = 0 THEN RETURN
    #nw_avail = (PEEK(FN_RX) AND 255) + (PEEK(FN_RX + 1) AND 255) * 256
    ' 16-bit compares are signed: 32768 and up would read as negative
    IF (#nw_avail AND $8000) <> 0 THEN #nw_avail = $7FFF
    #fn_t = PEEK(FN_RX + 2) AND 255
    nw_conn = #fn_t
    #fn_t = PEEK(FN_RX + 3) AND 255
    nw_err = #fn_t
END

' network_bus_read: #cp_n bytes (1-512) to #cp_dst -> fn_ok
net_bus_read: PROCEDURE
    IF #cp_n > 512 THEN #cp_n = 512
    mb_dev = NET_DEV0 + nw_unit
    mb_cmd = NETCMD_READ
    mb_nparam = 1
    pm_i = 0 : pm_size = 2 : #pm_val = #cp_n : GOSUB fn_param
    #fn_txlen = 0
    GOSUB fn_transact
    IF fn_ok THEN GOSUB rx_copy
END

' network_read_nb: wait for data, then read what is there, up to #cp_n,
' to #cp_dst. #rd_n = bytes read; rd_bad = a status or read call failed.
' Gives up after about a minute of nothing, which the C loop never does.
net_read_nb: PROCEDURE
    rd_bad = 0
    #rd_n = 0
    #nw_chunk = 0
    DO
        GOSUB net_status
        IF fn_ok = 0 THEN
            rd_bad = 1
            RETURN
        END IF
        IF #nw_avail = 0 THEN
            IF nw_err > 1 THEN RETURN
        END IF
        #nw_chunk = #nw_chunk + 1
        IF #nw_chunk > 3600 THEN RETURN
    LOOP WHILE #nw_avail = 0
    IF #cp_n > #nw_avail THEN #cp_n = #nw_avail
    IF #cp_n > 512 THEN #cp_n = 512
    #nw_left = #cp_n
    GOSUB net_bus_read
    IF fn_ok = 0 THEN
        rd_bad = 1
        RETURN
    END IF
    #rd_n = #nw_left
END

' network_read: loop net_read_nb until #cp_n bytes or no more data.
' #rd_n = total.
net_read: PROCEDURE
    #nw_total = 0
    WHILE #cp_n > 0
        GOSUB net_read_nb
        IF #rd_n < 1 THEN
            #rd_n = #nw_total
            RETURN
        END IF
        #nw_total = #nw_total + #rd_n
        #cp_n = #cp_n - #rd_n
        #cp_dst = #cp_dst + #rd_n
    WEND
    #rd_n = #nw_total
END

' network_write: #cp_n bytes from #fn_src, in packets of up to 256 -> fn_ok
net_write: PROCEDURE
    #nw_left = #cp_n
    fn_ok = 1
    WHILE #nw_left > 0
        IF #nw_left > 256 THEN
            #nw_chunk = 256
        ELSE
            #nw_chunk = #nw_left
        END IF
        mb_dev = NET_DEV0 + nw_unit
        mb_cmd = NETCMD_WRITE
        mb_nparam = 1
        pm_i = 0 : pm_size = 2 : #pm_val = #nw_chunk : GOSUB fn_param
        #cp_n = #nw_chunk
        GOSUB tx_put
        GOSUB fn_transact
        IF fn_ok = 0 THEN RETURN
        #fn_src = #fn_src + #nw_chunk
        #nw_left = #nw_left - #nw_chunk
    WEND
END

' network_fs_cd(spec) -> fn_ok
net_fs_cd: PROCEDURE
    GOSUB net_unit_of
    mb_dev = NET_DEV0 + nw_unit
    mb_cmd = NETCMD_CHDIR
    mb_nparam = 0
    #cp_n = #cs_len
    GOSUB tx_put
    GOSUB fn_transact
END

' network_fs_pwd(spec) -> fn_ok, directory at NB_PATH (NUL-terminated)
net_fs_pwd: PROCEDURE
    GOSUB net_unit_of
    mb_cmd = NETCMD_GETCWD
    GOSUB net_cmd
    IF fn_ok = 0 THEN RETURN
    #cp_dst = NB_PATH : #cp_n = 127
    GOSUB rx_to_buf
END

' network_set_eol: the #cs_len bytes at #fn_src; none sends no payload.
' (fuji_devcall_vld)
    CONST NETCMD_SET_EOL = $4C
net_set_eol: PROCEDURE
    mb_dev = NET_DEV0 + nw_unit
    mb_cmd = NETCMD_SET_EOL
    mb_nparam = 0
    #cp_n = #cs_len
    GOSUB tx_put
    GOSUB fn_transact
END

' ---- the commands added in lib v5 ----
    CONST NETCMD_SET_PARSER = $FC
    CONST NETCMD_PARSE      = $50
    CONST NETCMD_QUERY      = $51
    CONST NETCMD_TRANS      = $54
    CONST NETCMD_USERNAME   = $FD
    CONST NETCMD_PASSWORD   = $FE
    CONST NETCMD_INT_RATE   = $5A
    CONST NETCMD_ACCEPT     = $41
    CONST NETCMD_CLOSE_CLI  = $63
    CONST NETCMD_SEEK       = $25
    CONST NETCMD_TELL       = $26
    CONST NETCMD_DEST       = $44
    CONST NETCMD_REMOTE     = $72

    DIM nw_a
    DIM #nw_pos, #nw_poshi

' Command mb_cmd with the byte nw_a as both parameters (A1_A2)
net_a12: PROCEDURE
    mb_dev = NET_DEV0 + nw_unit
    mb_nparam = 2
    pm_i = 0 : pm_size = 1 : #pm_val = nw_a : GOSUB fn_param
    pm_i = 1 : pm_size = 1 : #pm_val = nw_a : GOSUB fn_param
    #fn_txlen = 0
    GOSUB fn_transact
END

' network_set_parser(unit, mode nw_a)
net_set_parser: PROCEDURE
    mb_cmd = NETCMD_SET_PARSER
    GOSUB net_a12
END

net_parse: PROCEDURE
    mb_cmd = NETCMD_PARSE
    GOSUB net_cmd
END

' network_json_parse: JSON parser, then PARSE; fn_ok only if both worked
net_json_parse: PROCEDURE
    nw_a = 1
    GOSUB net_set_parser
    IF fn_ok = 0 THEN RETURN
    GOSUB net_parse
END

' Command mb_cmd with params 0,0 and the #cs_len bytes at #fn_src
' (username, password, fs commands)
net_str0: PROCEDURE
    mb_dev = NET_DEV0 + nw_unit
    mb_nparam = 2
    pm_i = 0 : pm_size = 1 : #pm_val = 0 : GOSUB fn_param
    pm_i = 1 : pm_size = 1 : #pm_val = 0 : GOSUB fn_param
    #cp_n = #cs_len
    GOSUB tx_put
    GOSUB fn_transact
END

' network_seek(unit, #nw_pos): one 4-byte parameter
net_seek: PROCEDURE
    mb_dev = NET_DEV0 + nw_unit
    mb_cmd = NETCMD_SEEK
    mb_nparam = 1
    POKE FN_PARAM_SIZE, 4
    POKE FN_PARAM_VAL, #nw_pos AND 255
    POKE FN_PARAM_VAL + 1, #nw_pos / 256
    POKE FN_PARAM_VAL + 2, #nw_poshi AND 255
    POKE FN_PARAM_VAL + 3, #nw_poshi / 256
    #fn_txlen = 0
    GOSUB fn_transact
END

' network_tell(unit) -> #nw_pos (low 16 bits), #nw_poshi
net_tell: PROCEDURE
    mb_cmd = NETCMD_TELL
    GOSUB net_cmd
    #nw_pos = $FFFF : #nw_poshi = $FFFF
    IF fn_ok THEN
        #nw_pos = (PEEK(FN_RX) AND 255) + (PEEK(FN_RX + 1) AND 255) * 256
        #nw_poshi = (PEEK(FN_RX + 2) AND 255) + (PEEK(FN_RX + 3) AND 255) * 256
    END IF
END

' ---- shared by the network test programs ----
    DIM wd_go, sh_m
    DIM #wd_r, #wd_bw, #wd_lim

' Poll STATUS on nw_unit up to #wd_lim times, until data, an error or a
' status failure -> #wd_bw bytes waiting
wait_for_data: PROCEDURE
    #wd_r = 0
    #wd_bw = 0
    wd_go = 1
    WHILE wd_go
        GOSUB net_status
        #wd_bw = #nw_avail
        IF fn_ok = 0 THEN wd_go = 0
        IF #nw_avail > 0 THEN wd_go = 0
        IF nw_err <> 1 THEN wd_go = 0
        #wd_r = #wd_r + 1
        IF #wd_r >= #wd_lim THEN wd_go = 0
    WEND
END

' t_in = the NUL-terminated RAM string at #sa holds the #sn bytes at #sb
str_has: PROCEDURE
    t_in = 0
    #x2 = 0
    WHILE (PEEK(#sa + #x2) AND 255) <> 0
        sh_m = 1
        FOR #x3 = 0 TO #sn - 1
            IF (PEEK(#sa + #x2 + #x3) AND 255) <> (PEEK(#sb + #x3) AND 255) THEN sh_m = 0
        NEXT #x3
        IF sh_m THEN t_in = 1
        #x2 = #x2 + 1
    WEND
END


    DIM jq_bad
    DIM #jq_n

' network_json_query: the path at #fn_src/#cs_len (sent NUL-padded to the
' lib's 256), reply drained into NB_BUF. #jq_n = length, trailing LF/CR/9B
' stripped; jq_bad = the call failed (the lib's negative return).
net_json_query: PROCEDURE
    jq_bad = 0
    #jq_n = 0
    mb_dev = NET_DEV0 + nw_unit
    mb_cmd = NETCMD_QUERY
    mb_nparam = 0
    #cp_n = #cs_len
    GOSUB tx_put
    FOR #fn_i = #cs_len TO 255
        POKE FN_TX + #fn_i, 0
    NEXT #fn_i
    #fn_txlen = 256
    GOSUB fn_transact
    IF fn_ok = 0 THEN
        jq_bad = 1
        RETURN
    END IF
    wd_go = 1
    WHILE wd_go
        GOSUB net_status
        IF fn_ok = 0 THEN
            jq_bad = 1
            wd_go = 0
        ELSEIF #nw_avail = 0 THEN
            wd_go = 0
        ELSE
            #cp_dst = NB_BUF + #jq_n : #cp_n = #nw_avail
            GOSUB net_read
            IF #rd_n < 1 THEN
                jq_bad = 1
                wd_go = 0
            ELSE
                #jq_n = #jq_n + #rd_n
            END IF
        END IF
    WEND
    IF jq_bad THEN RETURN
    wd_go = 1
    WHILE wd_go
        wd_go = 0
        IF #jq_n > 0 THEN
            #fn_t = PEEK(NB_BUF + #jq_n - 1) AND 255
            IF #fn_t = $9B THEN wd_go = 1
            IF #fn_t = $0A THEN wd_go = 1
            IF #fn_t = $0D THEN wd_go = 1
            IF wd_go THEN #jq_n = #jq_n - 1
        END IF
    WEND
    POKE NB_BUF + #jq_n, 0
END

' network_html_parse: HTML parser (2), then PARSE
net_html_parse: PROCEDURE
    nw_a = 2
    GOSUB net_set_parser
    IF fn_ok = 0 THEN RETURN
    GOSUB net_parse
END

' network_set_query_param(unit, value nw_a): SET_PARAMETER a1 = 0, a2 = value
    CONST NETCMD_SET_PARAM = $FB
net_set_query_param: PROCEDURE
    mb_dev = NET_DEV0 + nw_unit
    mb_cmd = NETCMD_SET_PARAM
    mb_nparam = 2
    pm_i = 0 : pm_size = 1 : #pm_val = 0 : GOSUB fn_param
    pm_i = 1 : pm_size = 1 : #pm_val = nw_a : GOSUB fn_param
    #fn_txlen = 0
    GOSUB fn_transact
END

' network_http_set_mode(unit, mode nw_a): SET_HTTP_MODE a1 = 0, a2 = mode
    CONST NETCMD_HTTP_MODE = $4D
net_http_mode: PROCEDURE
    mb_dev = NET_DEV0 + nw_unit
    mb_cmd = NETCMD_HTTP_MODE
    mb_nparam = 2
    pm_i = 0 : pm_size = 1 : #pm_val = 0 : GOSUB fn_param
    pm_i = 1 : pm_size = 1 : #pm_val = nw_a : GOSUB fn_param
    #fn_txlen = 0
    GOSUB fn_transact
END

' network_http_post / put of the #cp_n bytes at #fn_src: set-data mode,
' write, back to body mode -> fn_ok
net_http_post: PROCEDURE
    #nw_total = #cp_n
    nw_a = 4
    GOSUB net_http_mode
    IF fn_ok = 0 THEN RETURN
    #cp_n = #nw_total
    GOSUB net_write
    IF fn_ok = 0 THEN RETURN
    nw_a = 0
    GOSUB net_http_mode
END
