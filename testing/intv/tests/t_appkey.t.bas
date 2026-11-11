' appkey: tests/appkey.c
    DIM ta_i, ta_same

test_appkey: PROCEDURE
    SECTION "fuji appkey r/w"
    #ak_creator = $5445 : ak_app = 1 : #fn_src = AK_WRITE

    FOR ta_i = 0 TO 63
        POKE AK_WRITE + ta_i, $AB
    NEXT ta_i
    POKE AK_WRITE, 1 : POKE AK_WRITE + 63, 2
    ak_key = 0 : #cs_len = 64
    GOSUB fuji_write_appkey
    CHECK "write 1 ok" @56, fn_ok

    GOSUB ak_clear_read
    ak_key = 0
    GOSUB fuji_read_appkey
    CHECK "read 1 ok" @61, fn_ok
    CHECK "read 1 count" @62, #cs_len = 64
    GOSUB ak_cmp64
    CHECK "read 1 data" @66, ta_same

    FOR ta_i = 0 TO 63
        POKE AK_WRITE + ta_i, $54
    NEXT ta_i
    POKE AK_WRITE, 3 : POKE AK_WRITE + 63, 4
    ak_key = 0 : #cs_len = 64
    GOSUB fuji_write_appkey
    CHECK "write 2 ok" @75, fn_ok

    GOSUB ak_clear_read
    ak_key = 0
    GOSUB fuji_read_appkey
    CHECK "read 2 ok" @80, fn_ok
    CHECK "read 2 count" @81, #cs_len = 64
    GOSUB ak_cmp64
    CHECK "read 2 data" @82, ta_same

    ak_key = $10 : #cs_len = 0
    GOSUB fuji_write_appkey
    CHECK "write empty ok" @87, fn_ok

    GOSUB ak_sentinel
    ak_key = $10
    GOSUB fuji_read_appkey
    CHECK "read empty ok" @92, fn_ok
    CHECK "empty len ok" @94, #cs_len <= 64
    GOSUB ak_zeroed
    CHECK "empty no junk" @96, ta_same

    ak_key = $10 : #fn_src = AK_READ
    GOSUB fuji_write_appkey
    #fn_src = AK_WRITE
    CHECK "write back ok" @101, fn_ok

    GOSUB ak_sentinel
    ak_key = $10
    GOSUB fuji_read_appkey
    CHECK "read back ok" @106, fn_ok
    CHECK "back len ok" @108, #cs_len <= 64
    GOSUB ak_zeroed
    CHECK "back no junk" @110, ta_same

    GOSUB ak_sentinel
    ak_key = $11
    GOSUB fuji_read_appkey
    CHECK "read missing ok" @117, fn_ok
    CHECK "missing empty" @119, #cs_len = 0
END

' Read buffer filled with a non-zero sentinel and the count likewise, so
' "came back zeroed" cannot pass on a buffer we zeroed.
ak_sentinel: PROCEDURE
    FOR ta_i = 0 TO 65
        POKE AK_READ + ta_i, $EE
    NEXT ta_i
    #cs_len = $EE
END

ak_clear_read: PROCEDURE
    FOR ta_i = 0 TO 65
        POKE AK_READ + ta_i, 0
    NEXT ta_i
    #cs_len = 0
END

' ta_same = AK_READ[0..63] equals AK_WRITE[0..63]
ak_cmp64: PROCEDURE
    ta_same = 1
    FOR ta_i = 0 TO 63
        IF (PEEK(AK_READ + ta_i) AND 255) <> (PEEK(AK_WRITE + ta_i) AND 255) THEN ta_same = 0
    NEXT ta_i
END

' ta_same = the first #cs_len bytes of AK_READ are zero (and #cs_len <= 64)
ak_zeroed: PROCEDURE
    ta_same = 1
    IF #cs_len > 64 THEN
        ta_same = 0
        RETURN
    END IF
    IF #cs_len > 0 THEN
        FOR ta_i = 1 TO #cs_len
            IF (PEEK(AK_READ + ta_i - 1) AND 255) <> 0 THEN ta_same = 0
        NEXT ta_i
    END IF
END
