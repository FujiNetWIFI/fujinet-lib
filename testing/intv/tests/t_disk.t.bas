' disk: tests/disk.c
    CONST DK_HOSTS = $9300      ' 256: host slots
    CONST DK_DEVS  = $9400      ' 304: device slots
    CONST DK_ENT   = $9540      ' 64: a directory entry
    CONST DK_SAVED = $9580      ' 32: the borrowed host slot
    CONST DK_SRCSZ = $95A0      ' 4: source size
    CONST DK_USED  = $95C0      ' 8: host slot n is read by a mounted disk
    DIM dk_sd, dk_src, dk_pre, dk_found, dk_ok, dk_i, dk_fsh
    DIM dk_existed, dk_pday, dk_pmin, dk_copy, dk_try
    DIM #dk_a, #dk_b

STR tnfs_host "tnfs.fujinet.online"
STR sd_name "SD"
STR dir_root "/"
STR dir_coco "/COCO"
STR f_lobby "lobby.dsk"
STR f_missing "abcdefghijklm.dsk"
STR copy_spec "COCO/lobby.dsk|lobby.dsk"
STR new_url "N1:SD:///fnnewdisk.img"

' fuji_file_stat(dk_fsh, dir #x4/#x5, name #x6/#x7) -> dk_found, entry at DK_ENT
' (day at +2, minute at +4, size at +6..+9)
fs_stat: PROCEDURE
    dk_found = 0
    mb_cmd = $F9 : fz_a = dk_fsh
    GOSUB fd_a1
    ' open the directory: path, NUL, empty filter, padded to 256
    mb_cmd = $F7 : fz_n = 1 : fz_a = dk_fsh
    GOSUB fz_set
    #fn_src = #x4 : #cs_len = #x5 : GOSUB fz_pad
    GOSUB fz_go
    IF fn_ok = 0 THEN RETURN
    #dk_a = 12 + #x7 + 1                ' the entry size asked for
    wd_go = 1
    WHILE wd_go
        #cp_dst = DK_ENT : #cp_n = 64 : GOSUB buf_zero
        mb_cmd = $F6 : fz_n = 2 : fz_a = #dk_a : fz_b = $80
        GOSUB fz_set
        GOSUB fz_go
        IF fn_ok = 0 THEN
            wd_go = 0
        ELSE
            #cp_dst = DK_ENT : #cp_n = #dk_a : GOSUB rx_copy
            IF (PEEK(DK_ENT + 1) AND 255) = $7F THEN
                wd_go = 0
            ELSE
                #sa = DK_ENT + 12 : #sb = #x6 : #sn = #x7 + 1 : GOSUB str_eq
                IF t_same THEN
                    dk_found = 1
                    wd_go = 0
                END IF
            END IF
        END IF
    WEND
    mb_cmd = $F5
    GOSUB fd_plain
END

' dk_stat_sd: stat the SD host's /lobby.dsk -> dk_found
dk_stat_lobby_sd: PROCEDURE
    dk_fsh = dk_sd
    #x4 = VARPTR str_dir_root(0) : #x5 = SL_dir_root
    #x6 = VARPTR str_f_lobby(0) : #x7 = SL_f_lobby
    GOSUB fs_stat
END

' fuji_copy_file(dk_src+1, dk_sd+1, COCO/lobby.dsk|lobby.dsk) -> dk_copy
dk_copy_file: PROCEDURE
    mb_cmd = $D8 : fz_n = 2 : fz_a = dk_src + 1 : fz_b = dk_sd + 1
    GOSUB fz_set
    #fn_src = VARPTR str_copy_spec(0) : #cs_len = SL_copy_spec : GOSUB fz_pad
    GOSUB fz_go
    dk_copy = fn_ok
    IF dk_copy THEN RETURN
    ' the copy can outlast the bus call: wait for the device, then compare sizes
    dk_try = 0
    wd_go = 1
    WHILE wd_go
        mb_cmd = $53 : fz_a = 0
        GOSUB fd_a1
        IF fn_ok THEN wd_go = 0
        IF wd_go THEN
            dk_try = dk_try + 1
            IF dk_try >= 100 THEN wd_go = 0
        END IF
    WEND
    IF dk_try = 0 THEN RETURN
    IF dk_try >= 100 THEN RETURN
    dk_fsh = dk_src
    #x4 = VARPTR str_dir_coco(0) : #x5 = SL_dir_coco
    #x6 = VARPTR str_f_lobby(0) : #x7 = SL_f_lobby
    GOSUB fs_stat
    IF dk_found = 0 THEN RETURN
    FOR dk_i = 0 TO 3
        POKE DK_SRCSZ + dk_i, PEEK(DK_ENT + 6 + dk_i) AND 255
    NEXT dk_i
    GOSUB dk_stat_lobby_sd
    IF dk_found = 0 THEN RETURN
    dk_copy = 1
    FOR dk_i = 0 TO 3
        IF (PEEK(DK_ENT + 6 + dk_i) AND 255) <> (PEEK(DK_SRCSZ + dk_i) AND 255) THEN dk_copy = 0
    NEXT dk_i
END

test_fuji_copy_file: PROCEDURE
    SECTION "fuji_copy_file"
    #cp_dst = DK_HOSTS : #cp_n = 256 : GOSUB buf_zero
    mb_cmd = $F4
    GOSUB fd_plain
    dk_ok = fn_ok
    #cp_dst = DK_HOSTS : #cp_n = 256 : GOSUB rx_copy
    CHECK "get hosts ok" @64, dk_ok

    dk_found = 0
    FOR dk_i = 0 TO 7
        #sa = DK_HOSTS + dk_i * 32 : #sb = VARPTR str_sd_name(0) : #sn = 3 : GOSUB str_eq
        IF t_same THEN
            IF dk_found = 0 THEN
                dk_sd = dk_i
                dk_found = 1
            END IF
        END IF
    NEXT dk_i
    CHECK "SD slot found" @74, dk_found

    ' device slots in use by a mounted disk
    FOR dk_i = 0 TO 7
        POKE DK_USED + dk_i, 0
    NEXT dk_i
    #cp_dst = DK_DEVS : #cp_n = 304 : GOSUB buf_zero
    mb_cmd = $F2
    GOSUB fd_plain
    IF fn_ok THEN
        #cp_dst = DK_DEVS : #cp_n = 304 : GOSUB rx_copy
        FOR dk_i = 0 TO 7
            IF (PEEK(DK_DEVS + dk_i * 38 + 2) AND 255) <> 0 THEN
                #dk_b = PEEK(DK_DEVS + dk_i * 38) AND 255
                IF #dk_b < 8 THEN POKE DK_USED + #dk_b, 1
            END IF
        NEXT dk_i
    END IF

    ' the source host: one already named tnfs.fujinet.online, else an empty
    ' slot, else any slot no mounted disk reads from
    dk_pre = 0 : dk_src = 0 : dk_ok = 0
    FOR dk_i = 0 TO 7
        #sa = DK_HOSTS + dk_i * 32 : #sb = VARPTR str_tnfs_host(0) : #sn = SL_tnfs_host + 1 : GOSUB str_eq
        IF t_same THEN
            IF dk_ok = 0 THEN
                dk_src = dk_i
                dk_ok = 1
                dk_pre = 1
            END IF
        END IF
    NEXT dk_i
    IF dk_ok = 0 THEN
        FOR dk_i = 0 TO 7
            IF (PEEK(DK_HOSTS + dk_i * 32) AND 255) = 0 THEN
                IF dk_ok = 0 THEN
                    dk_src = dk_i
                    dk_ok = 1
                END IF
            END IF
        NEXT dk_i
    END IF
    IF dk_ok = 0 THEN
        FOR dk_i = 0 TO 7
            IF dk_i <> dk_sd THEN
                IF (PEEK(DK_USED + dk_i) AND 255) = 0 THEN
                    IF dk_ok = 0 THEN
                        dk_src = dk_i
                        dk_ok = 1
                    END IF
                END IF
            END IF
        NEXT dk_i
    END IF

    IF dk_pre = 0 THEN
        ' borrow the slot: remember it, write the source host name
        FOR dk_i = 0 TO 31
            POKE DK_SAVED + dk_i, PEEK(DK_HOSTS + dk_src * 32 + dk_i) AND 255
            POKE DK_HOSTS + dk_src * 32 + dk_i, 0
        NEXT dk_i
        FOR dk_i = 0 TO SL_tnfs_host - 1
            POKE DK_HOSTS + dk_src * 32 + dk_i, PEEK(VARPTR str_tnfs_host(0) + dk_i) AND 255
        NEXT dk_i
        mb_cmd = $F3 : fz_n = 0
        GOSUB fz_set
        #fn_src = DK_HOSTS : #cp_n = 256 : GOSUB tx_put
        GOSUB fz_go
        CHECK "put hosts ok" @136, fn_ok
    END IF

    mb_cmd = $F9 : fz_a = dk_src
    GOSUB fd_a1
    CHECK "mount TNFS" @140, fn_ok
    mb_cmd = $F9 : fz_a = dk_sd
    GOSUB fd_a1
    CHECK "mount SD" @143, fn_ok

    GOSUB dk_stat_lobby_sd
    dk_existed = dk_found
    IF dk_found THEN
        #dk_b = PEEK(DK_ENT + 2) AND 255
        dk_pday = #dk_b
        #dk_b = PEEK(DK_ENT + 4) AND 255
        dk_pmin = #dk_b
    END IF

    dk_fsh = dk_sd
    #x4 = VARPTR str_dir_root(0) : #x5 = SL_dir_root
    #x6 = VARPTR str_f_missing(0) : #x7 = SL_f_missing
    GOSUB fs_stat
    CHECK "missing is NULL" @158, dk_found = 0

    dk_fsh = dk_src
    #x4 = VARPTR str_dir_coco(0) : #x5 = SL_dir_coco
    #x6 = VARPTR str_f_lobby(0) : #x7 = SL_f_lobby
    GOSUB fs_stat
    CHECK "found on TNFS" @161, dk_found
    #x0 = 0
    IF dk_found THEN
        FOR dk_i = 0 TO 3
            POKE DK_SRCSZ + dk_i, PEEK(DK_ENT + 6 + dk_i) AND 255
            #x0 = #x0 + (PEEK(DK_ENT + 6 + dk_i) AND 255)
        NEXT dk_i
    END IF
    CHECK "TNFS not empty" @166, #x0 > 0

    GOSUB dk_copy_file

    IF dk_pre = 0 THEN
        ' put the borrowed host slot back
        #cp_dst = DK_HOSTS : #cp_n = 256 : GOSUB buf_zero
        mb_cmd = $F4
        GOSUB fd_plain
        dk_ok = fn_ok
        #cp_dst = DK_HOSTS : #cp_n = 256 : GOSUB rx_copy
        CHECK "restore re-read" @176, dk_ok
        IF dk_ok THEN
            FOR dk_i = 0 TO 31
                POKE DK_HOSTS + dk_src * 32 + dk_i, PEEK(DK_SAVED + dk_i) AND 255
            NEXT dk_i
            mb_cmd = $F3 : fz_n = 0
            GOSUB fz_set
            #fn_src = DK_HOSTS : #cp_n = 256 : GOSUB tx_put
            GOSUB fz_go
        END IF
    END IF
    mb_cmd = $F9 : fz_a = dk_sd
    GOSUB fd_a1

    CHECK "copy ok" @185, dk_copy
    GOSUB dk_stat_lobby_sd
    CHECK "found on SD" @188, dk_found
    #x0 = 0
    IF dk_found THEN
        #x0 = 1
        FOR dk_i = 0 TO 3
            IF (PEEK(DK_ENT + 6 + dk_i) AND 255) <> (PEEK(DK_SRCSZ + dk_i) AND 255) THEN #x0 = 0
        NEXT dk_i
    END IF
    CHECK "size matches" @196, #x0
    IF dk_existed THEN
        #x0 = 0
        IF dk_found THEN
            IF (PEEK(DK_ENT + 2) AND 255) <> dk_pday THEN #x0 = 1
            IF (PEEK(DK_ENT + 4) AND 255) <> dk_pmin THEN #x0 = 1
        END IF
        CHECK "timestamp new" @198, #x0
    END IF
END

test_fuji_create_new: PROCEDURE
    SECTION "fuji_create_new"
    #cp_dst = DK_HOSTS : #cp_n = 256 : GOSUB buf_zero
    mb_cmd = $F4
    GOSUB fd_plain
    dk_ok = fn_ok
    #cp_dst = DK_HOSTS : #cp_n = 256 : GOSUB rx_copy
    CHECK "get hosts ok" @249, dk_ok
    dk_found = 0
    FOR dk_i = 0 TO 7
        #sa = DK_HOSTS + dk_i * 32 : #sb = VARPTR str_sd_name(0) : #sn = 3 : GOSUB str_eq
        IF t_same THEN
            IF dk_found = 0 THEN
                dk_sd = dk_i
                dk_found = 1
            END IF
        END IF
    NEXT dk_i
    CHECK "SD slot found" @260, dk_found

    #cp_dst = DK_DEVS : #cp_n = 304 : GOSUB buf_zero
    mb_cmd = $F2
    GOSUB fd_plain
    dk_ok = fn_ok
    #cp_dst = DK_DEVS : #cp_n = 304 : GOSUB rx_copy
    CHECK "get devslots ok" @263, dk_ok
    ' first drive slot with no image
    dk_try = 0
    wd_go = 1
    WHILE wd_go
        IF dk_try >= 8 THEN
            wd_go = 0
        ELSEIF (PEEK(DK_DEVS + dk_try * 38 + 2) AND 255) = 0 THEN
            wd_go = 0
        ELSE
            dk_try = dk_try + 1
        END IF
    WEND

    mb_cmd = $F9 : fz_a = dk_sd
    GOSUB fd_a1
    CHECK "mount SD" @273, fn_ok
    ' A stopped run leaves the image behind, and NEW DISK refuses to replace it
    nw_unit = 1
    #fn_src = VARPTR str_new_url(0) : #cs_len = SL_new_url
    mb_cmd = $21
    GOSUB net_str0

    ' NEW DISK takes a 262-byte request and the mailbox carries 256, so the
    ' creating checks cannot run here.
    SKIPCHK "create ok" @301
    SKIPCHK "image on SD" @304
    SKIPCHK "image size" @307
    SKIPCHK "create again" @318

    mb_cmd = $F2
    GOSUB fd_plain
    dk_ok = fn_ok
    #cp_dst = DK_DEVS : #cp_n = 304 : GOSUB buf_zero
    #cp_dst = DK_DEVS : #cp_n = 304 : GOSUB rx_copy
    CHECK "get devslots ok" @322, dk_ok
    mb_cmd = $E9 : fz_a = dk_try
    GOSUB fd_a1
    CHECK "slot left empty" @325, (PEEK(DK_DEVS + dk_try * 38 + 2) AND 255) = 0
    SKIPCHK "image deleted" @327
END

test_disk: PROCEDURE
    GOSUB test_fuji_copy_file : IF t_abort THEN RETURN
    GOSUB test_fuji_create_new
END
