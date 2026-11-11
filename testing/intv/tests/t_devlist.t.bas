' devlist: tests/devlist.c
    CONST DL_EXP = $9200        ' "/k-check-k"
    DIM dl_u, dl_ok, dl_i, dl_r

' "Nk:" for k = dl_u -> NB_SPEC
dl_spec_n: PROCEDURE
    POKE NB_SPEC, 78
    POKE NB_SPEC + 1, 48 + dl_u
    POKE NB_SPEC + 2, 58
    #fn_src = NB_SPEC : #cs_len = 3
END

' "Nk:/k-check-k" -> NB_SPEC; "/k-check-k" -> DL_EXP
dl_spec_chk: PROCEDURE
    GOSUB dl_spec_n
    POKE NB_SPEC + 3, 47
    POKE NB_SPEC + 4, 48 + dl_u
    FOR dl_i = 0 TO 6
        POKE NB_SPEC + 5 + dl_i, PEEK(VARPTR dl_chk(0) + dl_i) AND 255
    NEXT dl_i
    POKE NB_SPEC + 12, 48 + dl_u
    #cs_len = 13
    POKE DL_EXP, 47
    POKE DL_EXP + 1, 48 + dl_u
    FOR dl_i = 0 TO 6
        POKE DL_EXP + 2 + dl_i, PEEK(VARPTR dl_chk(0) + dl_i) AND 255
    NEXT dl_i
    POKE DL_EXP + 9, 48 + dl_u
    POKE DL_EXP + 10, 0
END
dl_chk:
    DATA 45, 99, 104, 101, 99, 107, 45, 0       ' "-check-"

test_fujinet_exists: PROCEDURE
    SECTION "FujiNet device"
    GOSUB fuji_get_adapter_config
    CHECK "fujinet online" @38, fn_ok
END

test_clock_exists: PROCEDURE
    SECTION "Clock device"
    cf_fmt = 2 : cf_alt = 0
    GOSUB clock_get_time
    CHECK "clock online" @52, fn_ok
END

test_network_exists: PROCEDURE
    SECTION "N: device"
    ' Give every unit a unique prefix, clearing any an aborted run left.
    dl_ok = 1
    FOR dl_u = 1 TO 8
        GOSUB dl_spec_n
        GOSUB net_fs_cd
        GOSUB dl_spec_chk
        GOSUB net_fs_cd
        IF fn_ok = 0 THEN
            dl_ok = 0
            EXIT FOR
        END IF
    NEXT dl_u
    CHECK "all respond" @79, dl_ok

    ' Each unit still has its own prefix.
    dl_ok = 1
    FOR dl_u = 1 TO 8
        GOSUB dl_spec_n
        GOSUB net_fs_pwd
        IF fn_ok = 0 THEN
            dl_ok = 0
            EXIT FOR
        END IF
        ' strip a trailing '/' past the first character
        dl_r = 0
        FOR dl_i = 0 TO 126
            IF (PEEK(NB_PATH + dl_i) AND 255) = 47 THEN dl_r = dl_i
            IF (PEEK(NB_PATH + dl_i) AND 255) = 0 THEN EXIT FOR
        NEXT dl_i
        IF dl_r > 1 THEN POKE NB_PATH + dl_r, 0
        GOSUB dl_spec_chk
        #sa = NB_PATH : #sb = DL_EXP : #sn = 11
        GOSUB str_eq
        CHECKI "Net %d online" 8 @96, dl_u, t_same
    NEXT dl_u
    CHECK "all online" @98, dl_ok

    ' Clear every prefix, since it is prepended to URLs.
    FOR dl_u = 1 TO 8
        GOSUB dl_spec_n
        GOSUB net_fs_cd
        IF fn_ok = 0 THEN EXIT FOR
    NEXT dl_u
END

test_devlist: PROCEDURE
    GOSUB test_fujinet_exists : IF t_abort THEN RETURN
    GOSUB test_clock_exists : IF t_abort THEN RETURN
    GOSUB test_network_exists
END
