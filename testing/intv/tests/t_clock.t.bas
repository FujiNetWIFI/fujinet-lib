' clock: tests/clock.c
    DIM e_ok, ok1, ok2

STR tz_ny "America/New_York"
STR tz_utc "UTC"
STR tz_est "EST5EDT,M3.2.0,M11.1.0"
STR tz_jst "JST-9"
STR z_utc0 "+0000"

test_clock_tz: PROCEDURE
    SECTION "clock tz get/set"
    #cp_dst = CLK_TZ0 : #cp_n = 64 : GOSUB buf_zero
    #cp_dst = CLK_TZ0
    GOSUB clock_get_tz
    CHECK "get_tz ok" @35, fn_ok
    CHECK "tz not empty" @36, (PEEK(CLK_TZ0) AND 255) <> 0

    #fn_src = VARPTR str_tz_ny(0) : #cs_len = SL_tz_ny
    GOSUB clock_set_tz
    CHECK "set_tz ny ok" @44, fn_ok

    #cp_dst = CLK_TZ1 : #cp_n = 64 : GOSUB buf_zero
    #cp_dst = CLK_TZ1
    GOSUB clock_get_tz
    CHECK "get_tz again ok" @48, fn_ok
    #sa = CLK_TZ1 : #sb = VARPTR str_tz_ny(0) : #sn = SL_tz_ny + 1
    GOSUB str_eq
    CHECK "tz read back" @50, t_same

    #fn_src = CLK_TZ0
    GOSUB str_len
    #cs_len = #cp_n
    GOSUB clock_set_tz
END

test_clock_simple_binary: PROCEDURE
    SECTION "get_time SIMPLE"
    cf_fmt = 0 : cf_alt = 0
    GOSUB clock_get_time
    CHECK "simple ok" @70, fn_ok
    CHECK "simple century" @71, (PEEK(CLK_BUF) AND 255) = 20
    RANGE "simple year" @72, PEEK(CLK_BUF + 1) AND 255, 0, 99
    RANGE "simple month" @73, PEEK(CLK_BUF + 2) AND 255, 1, 12
    RANGE "simple day" @74, PEEK(CLK_BUF + 3) AND 255, 1, 31
    RANGE "simple hour" @75, PEEK(CLK_BUF + 4) AND 255, 0, 23
    RANGE "simple min" @76, PEEK(CLK_BUF + 5) AND 255, 0, 59
    RANGE "simple sec" @77, PEEK(CLK_BUF + 6) AND 255, 0, 59
END

test_clock_prodos_binary: PROCEDURE
    SECTION "get_time PRODOS"
    cf_fmt = 1 : cf_alt = 0
    GOSUB clock_get_time
    CHECK "prodos ok" @98, fn_ok
    #x0 = (PEEK(CLK_BUF) AND 255) + (PEEK(CLK_BUF + 1) AND 255) + (PEEK(CLK_BUF + 2) AND 255) + (PEEK(CLK_BUF + 3) AND 255)
    CHECK "prodos non-zero" @102, #x0 <> 0
END

test_clock_apetime_binary: PROCEDURE
    SECTION "get_time APETIME"
    cf_fmt = 2 : cf_alt = 0
    GOSUB clock_get_time
    CHECK "ape ok" @119, fn_ok
    RANGE "ape day" @120, PEEK(CLK_BUF) AND 255, 1, 31
    RANGE "ape month" @121, PEEK(CLK_BUF + 1) AND 255, 1, 12
    RANGE "ape year" @122, PEEK(CLK_BUF + 2) AND 255, 0, 99
    RANGE "ape hour" @123, PEEK(CLK_BUF + 3) AND 255, 0, 23
    RANGE "ape min" @124, PEEK(CLK_BUF + 4) AND 255, 0, 59
    RANGE "ape sec" @125, PEEK(CLK_BUF + 5) AND 255, 0, 59
END

test_clock_tz_iso_string: PROCEDURE
    SECTION "get_time TZ ISO"
    cf_fmt = 3 : cf_alt = 0
    GOSUB clock_get_time
    CHECK "iso ok" @146, fn_ok
    #x0 = 0
    IF (PEEK(CLK_BUF) AND 255) = 50 THEN
        IF (PEEK(CLK_BUF + 1) AND 255) = 48 THEN #x0 = 1
    END IF
    CHECK "iso starts 20" @147, #x0
    CHECK "iso T at 10" @148, (PEEK(CLK_BUF + 10) AND 255) = 84
    CHECK "iso colon 13" @149, (PEEK(CLK_BUF + 13) AND 255) = 58
    CHECK "iso colon 16" @150, (PEEK(CLK_BUF + 16) AND 255) = 58
    #fn_src = CLK_BUF
    GOSUB str_len
    CHECK "iso len 24" @152, #cp_n = 24
END

test_clock_utc_iso_string: PROCEDURE
    SECTION "get_time UTC ISO"
    cf_fmt = 4 : cf_alt = 0
    GOSUB clock_get_time
    CHECK "utc ok" @171, fn_ok
    #x0 = 0
    IF (PEEK(CLK_BUF) AND 255) = 50 THEN
        IF (PEEK(CLK_BUF + 1) AND 255) = 48 THEN #x0 = 1
    END IF
    CHECK "utc starts 20" @172, #x0
    #fn_src = CLK_BUF
    GOSUB str_len
    #x0 = 0
    IF #cp_n >= 5 THEN
        #sa = CLK_BUF + #cp_n - 5 : #sb = VARPTR str_z_utc0(0) : #sn = 5
        GOSUB str_eq
        #x0 = t_same
    END IF
    CHECK "utc ends +0000" @174, #x0
END

test_clock_apple3_sos: PROCEDURE
    SECTION "get_time SOS"
    cf_fmt = 5 : cf_alt = 0
    GOSUB clock_get_time
    CHECK "sos ok" @192, fn_ok
    CHECK "sos reserved 0" @194, (PEEK(CLK_BUF + 8) AND 255) = 48
END

test_clock_hundredths: PROCEDURE
    SECTION "get_time HUNDREDTHS"
    cf_fmt = 6 : cf_alt = 0
    GOSUB clock_get_time
    CHECK "hund ok" @211, fn_ok
    CHECK "hund century" @212, (PEEK(CLK_BUF) AND 255) = 20
    RANGE "hund year" @213, PEEK(CLK_BUF + 1) AND 255, 0, 99
    RANGE "hund month" @214, PEEK(CLK_BUF + 2) AND 255, 1, 12
    RANGE "hund day" @215, PEEK(CLK_BUF + 3) AND 255, 1, 31
    RANGE "hund hour" @216, PEEK(CLK_BUF + 4) AND 255, 0, 23
    RANGE "hund min" @217, PEEK(CLK_BUF + 5) AND 255, 0, 59
    RANGE "hund sec" @218, PEEK(CLK_BUF + 6) AND 255, 0, 59
    RANGE "hund hundredths" @219, PEEK(CLK_BUF + 7) AND 255, 0, 99
END

' Copy the 25-byte ISO string at CLK_BUF to #cp_dst.
clk_keep: PROCEDURE
    FOR #fn_i = 0 TO 24
        POKE #cp_dst + #fn_i, PEEK(CLK_BUF + #fn_i) AND 255
    NEXT #fn_i
END

test_clock_get_time_tz: PROCEDURE
    SECTION "get_time_tz"
    #cp_dst = CLK_UTC : #cp_n = 32 : GOSUB buf_zero
    #cp_dst = CLK_NY : GOSUB buf_zero
    #cp_dst = CLK_TOK : GOSUB buf_zero

    cf_fmt = 3
    #fn_src = VARPTR str_tz_utc(0) : #cs_len = SL_tz_utc
    GOSUB clock_get_time_tz
    #cp_dst = CLK_UTC : GOSUB clk_keep
    CHECK "tz utc ok" @242, fn_ok

    #fn_src = VARPTR str_tz_est(0) : #cs_len = SL_tz_est
    GOSUB clock_get_time_tz
    #cp_dst = CLK_NY : GOSUB clk_keep
    CHECK "tz east ok" @246, fn_ok

    #fn_src = VARPTR str_tz_jst(0) : #cs_len = SL_tz_jst
    GOSUB clock_get_time_tz
    #cp_dst = CLK_TOK : GOSUB clk_keep
    CHECK "tz tokyo ok" @250, fn_ok

    #sa = CLK_UTC : #sb = CLK_NY : #sn = 25
    GOSUB str_eq
    CHECK "utc vs east" @253, t_same = 0
    #sa = CLK_UTC : #sb = CLK_TOK : #sn = 25
    GOSUB str_eq
    CHECK "utc vs tokyo" @254, t_same = 0

    #cp_dst = CLK_SYS : #cp_n = 64 : GOSUB buf_zero
    #cp_dst = CLK_SYS
    GOSUB clock_get_tz
    #sa = CLK_SYS : #sb = VARPTR str_tz_jst(0) : #sn = SL_tz_jst + 1
    GOSUB str_eq
    CHECK "sys tz kept" @258, t_same = 0
END

' Read HUNDREDTHS: #x1 = minutes*60 + seconds, #x2 = hundredths,
' e_ok = the read succeeded. Hours drop out; a wrap past the hour is
' undone in clk_elapsed, which is enough for waits of seconds.
read_hundredths: PROCEDURE
    cf_fmt = 6 : cf_alt = 0
    GOSUB clock_get_time
    e_ok = fn_ok
    #x1 = (PEEK(CLK_BUF + 5) AND 255) * 60 + (PEEK(CLK_BUF + 6) AND 255)
    #x2 = PEEK(CLK_BUF + 7) AND 255
END

' #x5 = hundredths from (#x3, #x4) to the last reading
clk_elapsed: PROCEDURE
    IF #x1 >= #x3 THEN
        #x5 = #x1 - #x3
    ELSE
        #x5 = #x1 + 3600 - #x3
    END IF
    #x5 = #x5 * 100 + #x2
    #x5 = #x5 - #x4
END

' #x5 = #x5 - #x6, floored at zero
clk_less_overhead: PROCEDURE
    IF #x5 >= #x6 THEN
        #x5 = #x5 - #x6
    ELSE
        #x5 = 0
    END IF
END

test_sleep_wait: PROCEDURE
    SECTION "sleep timing"
    GOSUB read_hundredths
    #x3 = #x1 : #x4 = #x2 : ok1 = e_ok
    GOSUB read_hundredths
    ok2 = e_ok
    CHECK "ovh start ok" @315, ok1
    CHECK "ovh end ok" @316, ok2
    GOSUB clk_elapsed
    #x6 = #x5

    GOSUB read_hundredths
    #x3 = #x1 : #x4 = #x2 : ok1 = e_ok
    #cp_n = 1 : GOSUB sleep_secs
    GOSUB read_hundredths
    ok2 = e_ok
    CHECK "short start ok" @325, ok1
    CHECK "short end ok" @326, ok2
    GOSUB clk_elapsed
    GOSUB clk_less_overhead
    #x7 = #x5
    RANGE "short in tol" @332, #x5, 90, 110

    GOSUB read_hundredths
    #x3 = #x1 : #x4 = #x2 : ok1 = e_ok
    #cp_n = 10 : GOSUB sleep_secs
    GOSUB read_hundredths
    ok2 = e_ok
    CHECK "long start ok" @339, ok1
    CHECK "long end ok" @340, ok2
    GOSUB clk_elapsed
    GOSUB clk_less_overhead
    RANGE "long in tol" @345, #x5, 990, 1010
    #x5 = #x5 - #x7
    RANGE "long-short tol" @349, #x5, 890, 910
END

test_clock: PROCEDURE
    GOSUB test_clock_tz : IF t_abort THEN RETURN
    GOSUB test_clock_simple_binary : IF t_abort THEN RETURN
    GOSUB test_clock_prodos_binary : IF t_abort THEN RETURN
    GOSUB test_clock_apetime_binary : IF t_abort THEN RETURN
    GOSUB test_clock_tz_iso_string : IF t_abort THEN RETURN
    GOSUB test_clock_utc_iso_string : IF t_abort THEN RETURN
    GOSUB test_clock_apple3_sos : IF t_abort THEN RETURN
    GOSUB test_clock_hundredths : IF t_abort THEN RETURN
    GOSUB test_clock_get_time_tz : IF t_abort THEN RETURN
    GOSUB test_sleep_wait
END
