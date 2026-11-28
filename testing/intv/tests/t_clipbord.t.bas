' clipbord: tests/clipbord.c
    DIM cl_ok, cl_bad, cl_i

STR clip "N1:CLIPBOARD:///"
STR clip0 "N1:CLIPBOARD:///0"
STR clip1 "N1:CLIPBOARD:///1"
STR clip2 "N1:CLIPBOARD:///2"
STR clip10 "N1:CLIPBOARD:///10"
STR clipx "N1:CLIPBOARD:///x"
STR snip_a "FujiNet clipboard A"
STR snip_b "FujiNet clipboard B"
STR snip_c "FujiNet clipboard C"
STR snip_tail " plus tail"
STR snip_ct "FujiNet clipboard C plus tail"
STR just_x "x"

' clip_put: open the clipboard in nw_mode, write the #x4/#x5 text (none if
' #x5 is 0), close -> cl_ok
cl_put: PROCEDURE
    cl_ok = 0
    nw_trans = 0
    #fn_src = VARPTR str_clip(0) : #cs_len = SL_clip
    GOSUB net_open
    IF fn_ok = 0 THEN RETURN
    IF #x5 > 0 THEN
        #fn_src = #x4 : #cp_n = #x5 : GOSUB net_write
        IF fn_ok = 0 THEN
            GOSUB net_close
            RETURN
        END IF
    END IF
    GOSUB net_close
    cl_ok = fn_ok
END

' clip_get: read the spec at #x6/#x7 to NB_BUF -> #rd_n; cl_bad = open failed
cl_get: PROCEDURE
    #cp_dst = NB_BUF : #cp_n = 512 : GOSUB buf_zero
    cl_bad = 0
    #rd_n = 0
    nw_mode = 4 : nw_trans = 0
    #fn_src = #x6 : #cs_len = #x7
    GOSUB net_open
    IF fn_ok = 0 THEN
        cl_bad = 1
        RETURN
    END IF
    #cp_dst = NB_BUF : #cp_n = 511 : GOSUB net_read
    GOSUB net_close
END

' clip_is: the spec at #x6/#x7 reads back exactly the #x4/#x5 text -> cl_ok
cl_is: PROCEDURE
    GOSUB cl_get
    cl_ok = 0
    IF cl_bad = 0 THEN
        IF #rd_n = #x5 THEN
            #sa = NB_BUF : #sb = #x4 : #sn = #x5 : GOSUB str_eq
            cl_ok = t_same
        END IF
    END IF
END

test_clipboard: PROCEDURE
    SECTION "clipboard snippets"
    nw_mode = 8
    #x4 = VARPTR str_snip_a(0) : #x5 = SL_snip_a
    GOSUB cl_put
    CHECK "write snippet" @83, cl_ok
    #x6 = VARPTR str_clip(0) : #x7 = SL_clip
    GOSUB cl_is
    CHECK "/// reads back" @84, cl_ok
    #x6 = VARPTR str_clip0(0) : #x7 = SL_clip0
    GOSUB cl_is
    CHECK "///0 is same" @85, cl_ok

    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_clip(0) : #cs_len = SL_clip
    GOSUB net_open
    #x0 = fn_ok
    GOSUB net_status
    #x1 = 0
    IF #x0 THEN
        IF #nw_avail = SL_snip_a THEN
            IF nw_err = 1 THEN #x1 = 1
        END IF
    END IF
    CHECK "status all wait" @90, #x1
    #cp_dst = NB_BUF : #cp_n = SL_snip_a : GOSUB net_read
    #x2 = #rd_n
    GOSUB net_status
    #x1 = 0
    IF #x2 = SL_snip_a THEN
        IF #nw_avail = 0 THEN
            IF nw_err = 136 THEN #x1 = 1
        END IF
    END IF
    CHECK "status then EOF" @95, #x1
    GOSUB net_close

    nw_mode = 8
    #x4 = VARPTR str_snip_b(0) : #x5 = SL_snip_b
    GOSUB cl_put
    CHECK "write 2nd" @99, cl_ok
    #x4 = VARPTR str_snip_c(0) : #x5 = SL_snip_c
    GOSUB cl_put
    CHECK "write 3rd" @100, cl_ok
    #x6 = VARPTR str_clip(0) : #x7 = SL_clip
    GOSUB cl_is
    CHECK "/// newest" @101, cl_ok
    #x4 = VARPTR str_snip_b(0) : #x5 = SL_snip_b
    #x6 = VARPTR str_clip1(0) : #x7 = SL_clip1
    GOSUB cl_is
    CHECK "///1 before" @102, cl_ok
    #x4 = VARPTR str_snip_a(0) : #x5 = SL_snip_a
    #x6 = VARPTR str_clip2(0) : #x7 = SL_clip2
    GOSUB cl_is
    CHECK "///2 first" @103, cl_ok

    nw_mode = 9
    #x4 = VARPTR str_snip_tail(0) : #x5 = SL_snip_tail
    GOSUB cl_put
    CHECK "append" @105, cl_ok
    #x4 = VARPTR str_snip_ct(0) : #x5 = SL_snip_ct
    #x6 = VARPTR str_clip(0) : #x7 = SL_clip
    GOSUB cl_is
    CHECK "snippet + tail" @106, cl_ok
END

' open the spec at #fn_src/#cs_len with nw_mode, status error in nw_err
cl_try: PROCEDURE
    nw_trans = 0
    GOSUB net_open
    cl_ok = fn_ok
END

test_clipboard_refusals: PROCEDURE
    SECTION "clipboard refusals"
    nw_mode = 8
    #fn_src = VARPTR str_clip1(0) : #cs_len = SL_clip1
    GOSUB cl_try
    CHECK "old write open" @119, cl_ok = 0
    GOSUB net_status
    CHECK "denied 167" @120, nw_err = 167
    GOSUB net_close

    nw_mode = 4
    #fn_src = VARPTR str_clip(0) : #cs_len = SL_clip
    GOSUB cl_try
    CHECK "open to read" @124, cl_ok
    #fn_src = VARPTR str_just_x(0) : #cp_n = 1 : GOSUB net_write
    CHECK "wr on read bad" @126, fn_ok = 0
    GOSUB net_status
    CHECK "read only 135" @127, nw_err = 135
    GOSUB net_close

    nw_mode = 8
    #fn_src = VARPTR str_clip(0) : #cs_len = SL_clip
    GOSUB cl_try
    CHECK "open to write" @132, cl_ok
    nw_unit = 1
    #cp_dst = NB_BUF : #cp_n = 1 : GOSUB net_bus_read
    CHECK "read on write 0" @134, fn_ok = 0
    GOSUB net_status
    CHECK "write only 131" @135, nw_err = 131
    GOSUB net_close
    #x6 = VARPTR str_clip(0) : #x7 = SL_clip
    GOSUB cl_get
    #x0 = 0
    IF cl_bad = 0 THEN
        IF #rd_n = 0 THEN #x0 = 1
    END IF
    CHECK "empty write ok" @137, #x0

    nw_mode = 4
    #fn_src = VARPTR str_clip10(0) : #cs_len = SL_clip10
    GOSUB cl_try
    CHECK "index 10 fails" @140, cl_ok = 0
    GOSUB net_status
    CHECK "devspec 165 a" @141, nw_err = 165
    GOSUB net_close

    #fn_src = VARPTR str_clipx(0) : #cs_len = SL_clipx
    GOSUB cl_try
    CHECK "index x fails" @145, cl_ok = 0
    GOSUB net_status
    CHECK "devspec 165 b" @146, nw_err = 165
    GOSUB net_close

    nw_mode = 6
    #fn_src = VARPTR str_clip(0) : #cs_len = SL_clip
    GOSUB cl_try
    CHECK "dir open fails" @150, cl_ok = 0
    GOSUB net_status
    CHECK "invalid cmd 132" @151, nw_err = 132
    GOSUB net_close
END

test_clipboard_limit: PROCEDURE
    SECTION "clipboard limit"
    FOR #fn_i = 0 TO 511
        POKE NB_BUF + #fn_i, 97 + (#fn_i % 26)
    NEXT #fn_i
    nw_mode = 8
    #fn_src = VARPTR str_clip(0) : #cs_len = SL_clip
    GOSUB cl_try
    CHECK "open to write" @168, cl_ok

    nw_unit = 1
    #x1 = 0
    cl_ok = 1
    WHILE cl_ok
        IF #x1 >= 128 THEN
            cl_ok = 0
        ELSE
            #fn_src = NB_BUF : #cp_n = 512 : GOSUB net_write
            IF fn_ok = 0 THEN
                cl_ok = 0
            ELSE
                #x1 = #x1 + 1
            END IF
        END IF
    WEND
    CHECK "64 KB accepted" @172, #x1 = 128
    #fn_src = NB_BUF : #cp_n = 1 : GOSUB net_write
    CHECK "one more bad" @174, fn_ok = 0
    GOSUB net_status
    CHECK "no space 162" @175, nw_err = 162
    GOSUB net_close

    #x6 = VARPTR str_clip(0) : #x7 = SL_clip
    GOSUB cl_get
    #x0 = 1
    FOR #fn_i = 0 TO 510
        IF (PEEK(NB_BUF + #fn_i) AND 255) <> 97 + (#fn_i % 26) THEN #x0 = 0
    NEXT #fn_i
    CHECK "64 KB kept" @181, #x0

    nw_mode = 8
    #x4 = NB_BUF : #x5 = 0
    GOSUB cl_put
    CHECK "empty clipboard" @183, cl_ok
END

test_clipbord: PROCEDURE
    GOSUB test_clipboard : IF t_abort THEN RETURN
    GOSUB test_clipboard_refusals : IF t_abort THEN RETURN
    GOSUB test_clipboard_limit
END
