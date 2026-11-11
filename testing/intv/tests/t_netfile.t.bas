' netfile: tests/netfile.c and tests/ftp.c
    CONST FS_BASE = $9300       ' 96: <WEBDAV_ROOT><guid>/
    CONST FS_PATH = $9360       ' 128: the spec being built
    CONST FS_LIST = $93E0       ' 192: a directory listing
    CONST FS_DATA = $94A0       ' 32: a file's contents
    CONST FS_GUID = $94C0       ' 40
    DIM nf_i, nf_ok, nf_bl, nf_cnt, nf_col, nf_first, nf_z, nf_seen, nf_sd, nf_r
    DIM #nf_ll, #nf_tot

STR nf_webdav "N1:HTTPS://www.webdavserver.net/"
STR nf_never "N1:HTTPS://www.webdavserver.net/fnfstest-never-created/"
STR nf_slash "/"
STR nf_sub "sub/"
STR nf_alpha_p "sub/alpha.txt"
STR nf_beta_p "sub/beta.txt"
STR nf_gamma_p "sub/gamma.txt"
STR nf_delta_p "sub/delta.txt"
STR nf_halves_p "sub/halves.txt"
STR nf_ren_p "sub/gamma.txt,renamed.txt"
STR nf_renamed_p "sub/renamed.txt"
STR nf_doomed_p "doomed/"
STR nf_n1 "N1:"
STR nf_n1_base "N1:/"
STR nf_alpha "alpha"
STR nf_beta "beta"
STR nf_gamma "gamma"
STR nf_delta "delta"
STR nf_beta2 "beta-2"
STR nf_alpha_f "alpha.txt"
STR nf_beta_f "beta.txt"
STR nf_gamma_f "gamma.txt"
STR nf_delta_f "delta.txt"
STR nf_renamed_f "renamed.txt"
STR nf_first_h "first half,"
STR nf_second_h "second half"
STR nf_both_h "first half,second half"
STR nf_bigdir "N1:TNFS://apps.irata.online/Atari_8-bit/Games/Homesoft/"
STR nf_fdir "/fnfilter/"
STR nf_furl "N1:SD:///fnfilter/"
STR nf_fone "N1:SD:///fnfilter/one.txt"
STR nf_ftwo "N1:SD:///fnfilter/two.txt"
STR nf_one_f "one.txt"
STR nf_two_f "two.txt"
STR nf_one_d "one"
STR nf_two_d "two"
STR nf_filt "one.*"
STR nf_hdir "/fnhash/"
STR nf_hurl "N1:SD:///fnhash/"
STR nf_hfile "N1:SD:///fnhash/file#1.txt"
STR nf_hesc "N1:SD:///fnhash/file%231.txt"
STR nf_hplain "N1:SD:///fnhash/file"
STR nf_hname "file#1.txt"
STR nf_hshort "file"
STR nf_hdata "hash"
STR nf_lock_f "N1:TNFS://ayce.fozztexx.com/test/fnlocktest.txt"
STR nf_ftp_root "N1:FTP://ftp.untergrund.net/"
STR nf_ftp_miss "N1:FTP://ftp.untergrund.net/fnftptest-never-created/"
STR nf_ftp_crunch "N1:FTP://ftp.untergrund.net/breakpoint/2003/info/RESULTS.TXT"
STR nf_welcome "welcome.msg"
STR nf_robots "robots.txt"
STR nf_bpoint "breakpoint/"
STR nf_qqq "???"
STR nf_sd_name "SD"

' fs_cmd: command mb_cmd on the spec at #fn_src/#cs_len -> fn_ok
nf_cmd: PROCEDURE
    GOSUB net_unit_of
    GOSUB net_str0
END

' nf_path: FS_PATH = FS_BASE + the leaf at #x4/#x5; #fn_src/#cs_len = it
nf_path: PROCEDURE
    FOR #fn_i = 0 TO nf_bl - 1
        POKE FS_PATH + #fn_i, PEEK(FS_BASE + #fn_i) AND 255
    NEXT #fn_i
    FOR #fn_i = 0 TO #x5 - 1
        POKE FS_PATH + nf_bl + #fn_i, PEEK(#x4 + #fn_i) AND 255
    NEXT #fn_i
    POKE FS_PATH + nf_bl + #x5, 0
    #fn_src = FS_PATH
    #cs_len = nf_bl + #x5
END

' fs_put: create or replace the file at #fn_src/#cs_len with the #x6/#x7 data
' (an HTTP PUT, sent at the close) -> nf_ok
nf_put: PROCEDURE
    nf_ok = 0
    nw_mode = 8 : nw_trans = 0
    GOSUB net_open
    IF fn_ok = 0 THEN RETURN
    #fn_src = #x6 : #cp_n = #x7
    GOSUB net_http_post
    IF fn_ok = 0 THEN
        GOSUB net_close
        RETURN
    END IF
    GOSUB net_close
    nf_ok = fn_ok
END

' fs_get: read the file at #fn_src/#cs_len into FS_DATA -> #rd_n, nf_ok (opened)
nf_get: PROCEDURE
    nf_ok = 0
    nw_mode = 4 : nw_trans = 0
    GOSUB net_open
    IF fn_ok = 0 THEN
        #rd_n = 0
        RETURN
    END IF
    nf_ok = 1
    #cp_dst = FS_DATA : #cp_n = 32 : GOSUB buf_zero
    #cp_dst = FS_DATA : #cp_n = 31 : GOSUB net_read
    GOSUB net_close
END

' fs_list: open the directory #fn_src/#cs_len (mode nw_mode, format nw_trans),
' read it into FS_LIST with each line ending turned into a NUL -> nf_cnt
' entries, nf_ok = it opened, #nf_ll = bytes read
nf_list: PROCEDURE
    nf_ok = 0 : nf_cnt = 0 : #nf_ll = 0
    GOSUB net_open
    IF fn_ok = 0 THEN RETURN
    nf_ok = 1
    #cp_dst = FS_LIST : #cp_n = 192 : GOSUB buf_zero
    #cp_dst = FS_LIST : #cp_n = 191 : GOSUB net_read
    GOSUB net_close
    #nf_ll = #rd_n
    IF #nf_ll < 1 THEN RETURN
    FOR #fn_i = 0 TO #nf_ll - 1
        #fn_t = PEEK(FS_LIST + #fn_i) AND 255
        IF #fn_t = 13 THEN POKE FS_LIST + #fn_i, 0
        IF #fn_t = 10 THEN POKE FS_LIST + #fn_i, 0
        IF #fn_t = $9B THEN POKE FS_LIST + #fn_i, 0
    NEXT #fn_i
    #fn_i = 0
    WHILE #fn_i < #nf_ll
        IF (PEEK(FS_LIST + #fn_i) AND 255) <> 0 THEN
            nf_cnt = nf_cnt + 1
            WHILE (PEEK(FS_LIST + #fn_i) AND 255) <> 0
                #fn_i = #fn_i + 1
            WEND
        END IF
        #fn_i = #fn_i + 1
    WEND
END

' fs_listed: is the NUL-terminated name at #x6 (ROM, #x7 long) in the listing
' -> t_in
nf_listed: PROCEDURE
    t_in = 0
    #x2 = 0
    WHILE #x2 < #nf_ll
        IF (PEEK(FS_LIST + #x2) AND 255) <> 0 THEN
            #sa = FS_LIST + #x2 : #sb = #x6 : #sn = #x7 + 1 : GOSUB str_eq
            IF t_same THEN t_in = 1
            WHILE (PEEK(FS_LIST + #x2) AND 255) <> 0
                #x2 = #x2 + 1
            WEND
        END IF
        #x2 = #x2 + 1
    WEND
END

' FS_BASE = WEBDAV_ROOT + the GUID at FS_GUID + "/"; nf_bl = its length
nf_make_base: PROCEDURE
    FOR #fn_i = 0 TO SL_nf_webdav - 1
        POKE FS_BASE + #fn_i, PEEK(VARPTR str_nf_webdav(0) + #fn_i) AND 255
    NEXT #fn_i
    FOR #fn_i = 0 TO 35
        POKE FS_BASE + SL_nf_webdav + #fn_i, PEEK(FS_GUID + #fn_i) AND 255
    NEXT #fn_i
    POKE FS_BASE + SL_nf_webdav + 36, 47
    POKE FS_BASE + SL_nf_webdav + 37, 0
    nf_bl = SL_nf_webdav + 37
END

test_fs_make_test_dir: PROCEDURE
    SECTION "fs mkdir"
    CHECK "init ok" @174, 1
    #cp_dst = FS_GUID : #cp_n = 40 : GOSUB buf_zero
    mb_cmd = $BB
    GOSUB fd_plain
    nf_ok = fn_ok
    #cp_dst = FS_GUID : #cp_n = 37 : GOSUB rx_copy
    CHECK "guid ok" @182, nf_ok
    #fn_src = FS_GUID : GOSUB str_len
    CHECK "guid len 36" @183, #cp_n = 36
    GOSUB nf_make_base

    ' an earlier run that stopped before its teardown leaves it behind
    #fn_src = FS_BASE : #cs_len = nf_bl
    mb_cmd = $2B : GOSUB nf_cmd
    GOSUB net_close
    #fn_src = FS_BASE : #cs_len = nf_bl
    mb_cmd = $2A : GOSUB nf_cmd
    CHECK "mkdir ok" @201, fn_ok

    nw_mode = 6 : nw_trans = $83
    #fn_src = FS_BASE : #cs_len = nf_bl
    GOSUB net_open
    CHECK "new dir opens" @208, fn_ok
    GOSUB net_close

    nw_mode = 6 : nw_trans = $83
    #fn_src = VARPTR str_nf_never(0) : #cs_len = SL_nf_never
    GOSUB net_open
    CHECK "never made bad" @218, fn_ok = 0
    GOSUB net_close
END

test_fs_prefix: PROCEDURE
    SECTION "fs cd/pwd"
    #cp_dst = FS_GUID : #cp_n = 40 : GOSUB buf_zero
    mb_cmd = $BB
    GOSUB fd_plain
    #cp_dst = FS_GUID : #cp_n = 37 : GOSUB rx_copy
    ' FS_BASE = "N1:/<guid>"; FS_PATH = "/<guid>"
    POKE FS_BASE, 78 : POKE FS_BASE + 1, 49 : POKE FS_BASE + 2, 58 : POKE FS_BASE + 3, 47
    POKE FS_PATH, 47
    FOR #fn_i = 0 TO 36
        POKE FS_BASE + 4 + #fn_i, PEEK(FS_GUID + #fn_i) AND 255
        POKE FS_PATH + 1 + #fn_i, PEEK(FS_GUID + #fn_i) AND 255
    NEXT #fn_i

    nw_unit = 1
    #fn_src = VARPTR str_nf_n1(0) : #cs_len = 3
    mb_cmd = $2C : GOSUB net_str0
    mb_cmd = $2C : #fn_src = FS_BASE : #cs_len = 40 : GOSUB net_str0
    CHECK "cd ok" @261, fn_ok

    #fn_src = FS_BASE : GOSUB net_fs_pwd
    CHECK "getcwd ok" @264, fn_ok
    #x0 = 0
    FOR #fn_i = 0 TO 126
        IF (PEEK(NB_PATH + #fn_i) AND 255) = 47 THEN #x0 = #fn_i
        IF (PEEK(NB_PATH + #fn_i) AND 255) = 0 THEN EXIT FOR
    NEXT #fn_i
    IF #x0 > 1 THEN POKE NB_PATH + #x0, 0
    #sa = NB_PATH : #sb = FS_PATH : #sn = 38 : GOSUB str_eq
    CHECK "cwd matches" @271, t_same

    ' a full URL replaces the prefix rather than being appended to it
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_devspec(0) : #cs_len = SL_devspec
    GOSUB net_open
    CHECK "URL w/ prefix" @278, fn_ok
    #cp_dst = FS_LIST : #cp_n = 191 : GOSUB net_read
    CHECK "URL data" @280, #rd_n > 0
    GOSUB net_close

    ' reset the prefix, which would otherwise be prepended to relative specs
    nw_unit = 1
    #fn_src = VARPTR str_nf_n1(0) : #cs_len = 3
    mb_cmd = $2C : GOSUB net_str0
END

test_fs_create_files: PROCEDURE
    SECTION "fs subdir and files"
    #x4 = VARPTR str_nf_sub(0) : #x5 = SL_nf_sub : GOSUB nf_path
    mb_cmd = $2A : GOSUB nf_cmd
    CHECK "mkdir sub ok" @307, fn_ok
    nw_mode = 6 : nw_trans = $83
    GOSUB net_open
    CHECK "sub opens" @312, fn_ok
    GOSUB net_close

    #x4 = VARPTR str_nf_alpha_p(0) : #x5 = SL_nf_alpha_p : GOSUB nf_path
    #x6 = VARPTR str_nf_alpha(0) : #x7 = SL_nf_alpha : GOSUB nf_put
    CHECK "alpha created" @316, nf_ok
    #x4 = VARPTR str_nf_beta_p(0) : #x5 = SL_nf_beta_p : GOSUB nf_path
    #x6 = VARPTR str_nf_beta(0) : #x7 = SL_nf_beta : GOSUB nf_put
    CHECK "beta created" @319, nf_ok
    #x4 = VARPTR str_nf_gamma_p(0) : #x5 = SL_nf_gamma_p : GOSUB nf_path
    #x6 = VARPTR str_nf_gamma(0) : #x7 = SL_nf_gamma : GOSUB nf_put
    CHECK "gamma created" @322, nf_ok
    #x4 = VARPTR str_nf_delta_p(0) : #x5 = SL_nf_delta_p : GOSUB nf_path
    #x6 = VARPTR str_nf_delta(0) : #x7 = SL_nf_delta : GOSUB nf_put
    CHECK "delta created" @325, nf_ok

    #x4 = VARPTR str_nf_sub(0) : #x5 = SL_nf_sub : GOSUB nf_path
    nw_mode = 6 : nw_trans = $83
    GOSUB nf_list
    CHECK "list nonempty" @330, nf_cnt > 0
    #x6 = VARPTR str_nf_alpha_f(0) : #x7 = SL_nf_alpha_f : GOSUB nf_listed
    CHECK "alpha listed" @331, t_in
    #x6 = VARPTR str_nf_beta_f(0) : #x7 = SL_nf_beta_f : GOSUB nf_listed
    CHECK "beta listed" @332, t_in
    #x6 = VARPTR str_nf_gamma_f(0) : #x7 = SL_nf_gamma_f : GOSUB nf_listed
    CHECK "gamma listed" @333, t_in
    #x6 = VARPTR str_nf_delta_f(0) : #x7 = SL_nf_delta_f : GOSUB nf_listed
    CHECK "delta listed" @334, t_in
END

test_fs_directory_modes: PROCEDURE
    SECTION "fs dir modes"
    #x4 = VARPTR str_nf_sub(0) : #x5 = SL_nf_sub : GOSUB nf_path
    nw_unit = 1 : nw_a = 3
    mb_cmd = NETCMD_TRANS
    GOSUB net_a12
    CHECK "set trans ok" @358, fn_ok

    nw_mode = 6 : nw_trans = $80
    #fn_src = FS_PATH : #cs_len = nf_bl + SL_nf_sub
    GOSUB nf_list
    CHECK "DIR lists LONG" @362, nf_cnt > 0
    #x6 = VARPTR str_nf_alpha_f(0) : #x7 = SL_nf_alpha_f : GOSUB nf_listed
    CHECK "DIR keeps LONG" @363, t_in = 0

    nw_mode = 7 : nw_trans = $80
    #fn_src = FS_PATH : #cs_len = nf_bl + SL_nf_sub
    GOSUB nf_list
    CHECK "ALT lists LONG" @367, nf_cnt > 0
    #x6 = VARPTR str_nf_alpha_f(0) : #x7 = SL_nf_alpha_f : GOSUB nf_listed
    CHECK "ALT keeps LONG" @368, t_in = 0

    nw_mode = 7 : nw_trans = $83
    #fn_src = FS_PATH : #cs_len = nf_bl + SL_nf_sub
    GOSUB nf_list
    CHECK "ALT lists RAW" @372, nf_cnt > 0
    #x6 = VARPTR str_nf_alpha_f(0) : #x7 = SL_nf_alpha_f : GOSUB nf_listed
    CHECK "ALT RAW exact" @373, t_in

    nw_unit = 1 : nw_a = 0
    mb_cmd = NETCMD_TRANS
    GOSUB net_a12
END

test_fs_long_listing: PROCEDURE
    SECTION "fs LONG listing"
    nw_mode = 6 : nw_trans = $80
    #fn_src = VARPTR str_nf_bigdir(0) : #cs_len = SL_nf_bigdir
    GOSUB net_open
    CHECK "long dir opens" @403, fn_ok
    GOSUB net_status
    CHECK "status > 512" @408, #nw_avail > 512

    #x0 = 0                             ' total bytes
    nf_col = 0 : nf_z = 0 : nf_first = 0
    wd_go = 1
    WHILE wd_go
        #cp_dst = FS_LIST : #cp_n = 64 : GOSUB net_read
        IF #rd_n < 1 THEN
            wd_go = 0
        ELSE
            #x0 = #x0 + #rd_n
            FOR #fn_i = 0 TO #rd_n - 1
                #fn_t = PEEK(FS_LIST + #fn_i) AND 255
                IF #fn_t = 13 OR #fn_t = 10 OR #fn_t = $9B THEN
                    nf_col = 0
                ELSE
                    IF nf_col = 0 THEN
                        nf_first = #fn_t
                    ELSEIF nf_col = 1 THEN
                        IF nf_first = 90 THEN
                            IF #fn_t = 47 THEN nf_z = 1
                        END IF
                    END IF
                    IF nf_col < 250 THEN nf_col = nf_col + 1
                END IF
            NEXT #fn_i
        END IF
    WEND
    GOSUB net_close
    CHECK "listing > 512" @432, #x0 > 512
    CHECK "reaches Z/" @433, nf_z
END

test_fs_read_files: PROCEDURE
    SECTION "fs read/overwrite"
    #x4 = VARPTR str_nf_alpha_p(0) : #x5 = SL_nf_alpha_p : GOSUB nf_path
    GOSUB nf_get
    CHECK "alpha reads" @448, #rd_n > 0
    #sa = FS_DATA : #sb = VARPTR str_nf_alpha(0) : #sn = SL_nf_alpha + 1 : GOSUB str_eq
    CHECK "alpha matches" @449, t_same

    #x4 = VARPTR str_nf_beta_p(0) : #x5 = SL_nf_beta_p : GOSUB nf_path
    #x6 = VARPTR str_nf_beta2(0) : #x7 = SL_nf_beta2 : GOSUB nf_put
    CHECK "beta rewritten" @452, nf_ok
    #x4 = VARPTR str_nf_beta_p(0) : #x5 = SL_nf_beta_p : GOSUB nf_path
    GOSUB nf_get
    CHECK "beta reads" @453, #rd_n > 0
    #sa = FS_DATA : #sb = VARPTR str_nf_beta2(0) : #sn = SL_nf_beta2 + 1 : GOSUB str_eq
    CHECK "beta new data" @454, t_same
END

test_fs_put_body_writes: PROCEDURE
    SECTION "fs PUT in pieces"
    #x4 = VARPTR str_nf_halves_p(0) : #x5 = SL_nf_halves_p : GOSUB nf_path
    nw_mode = 8 : nw_trans = 0
    GOSUB net_open
    CHECK "open PUT" @476, fn_ok
    #fn_src = VARPTR str_nf_first_h(0) : #cp_n = 11 : GOSUB net_write
    CHECK "first write" @479, fn_ok
    GOSUB net_status
    #fn_src = VARPTR str_nf_second_h(0) : #cp_n = 11 : GOSUB net_write
    CHECK "second write" @485, fn_ok
    GOSUB net_close
    CHECK "close sends PUT" @488, fn_ok
    #x4 = VARPTR str_nf_halves_p(0) : #x5 = SL_nf_halves_p : GOSUB nf_path
    GOSUB nf_get
    CHECK "halves reads" @490, #rd_n = 22
    #sa = FS_DATA : #sb = VARPTR str_nf_both_h(0) : #sn = 23 : GOSUB str_eq
    CHECK "holds both" @491, t_same
END

test_fs_rename_delete: PROCEDURE
    SECTION "fs rename/delete"
    #x4 = VARPTR str_nf_ren_p(0) : #x5 = SL_nf_ren_p : GOSUB nf_path
    mb_cmd = $20 : GOSUB nf_cmd
    CHECK "rename ok" @511, fn_ok
    #x4 = VARPTR str_nf_renamed_p(0) : #x5 = SL_nf_renamed_p : GOSUB nf_path
    GOSUB nf_get
    CHECK "renamed reads" @514, #rd_n > 0
    #sa = FS_DATA : #sb = VARPTR str_nf_gamma(0) : #sn = SL_nf_gamma + 1 : GOSUB str_eq
    CHECK "keeps contents" @515, t_same

    #x4 = VARPTR str_nf_delta_p(0) : #x5 = SL_nf_delta_p : GOSUB nf_path
    mb_cmd = $21 : GOSUB nf_cmd
    CHECK "delete ok" @519, fn_ok

    #x4 = VARPTR str_nf_sub(0) : #x5 = SL_nf_sub : GOSUB nf_path
    nw_mode = 6 : nw_trans = $83
    GOSUB nf_list
    CHECK "list nonempty" @523, nf_cnt > 0
    #x6 = VARPTR str_nf_alpha_f(0) : #x7 = SL_nf_alpha_f : GOSUB nf_listed
    CHECK "alpha still" @524, t_in
    #x6 = VARPTR str_nf_renamed_f(0) : #x7 = SL_nf_renamed_f : GOSUB nf_listed
    CHECK "renamed listed" @525, t_in
    #x6 = VARPTR str_nf_gamma_f(0) : #x7 = SL_nf_gamma_f : GOSUB nf_listed
    CHECK "gamma gone" @526, t_in = 0
    #x6 = VARPTR str_nf_delta_f(0) : #x7 = SL_nf_delta_f : GOSUB nf_listed
    CHECK "delta gone" @527, t_in = 0
END

test_fs_dir_lifecycle: PROCEDURE
    SECTION "fs mkdir/rmdir"
    #x4 = VARPTR str_nf_doomed_p(0) : #x5 = SL_nf_doomed_p : GOSUB nf_path
    mb_cmd = $2A : GOSUB nf_cmd
    CHECK "mkdir doomed" @545, fn_ok
    nw_mode = 6 : nw_trans = $83
    GOSUB net_open
    CHECK "doomed opens" @549, fn_ok
    GOSUB net_close
    #fn_src = FS_PATH : #cs_len = nf_bl + SL_nf_doomed_p
    mb_cmd = $2B : GOSUB nf_cmd
    CHECK "rmdir ok" @553, fn_ok
    nw_mode = 6 : nw_trans = $83
    #fn_src = FS_PATH : #cs_len = nf_bl + SL_nf_doomed_p
    GOSUB net_open
    CHECK "gone fails" @557, fn_ok = 0
    GOSUB net_close
END

' the SD host slot index -> nf_sd (0 if it is not found)
nf_sd_host: PROCEDURE
    nf_sd = 0
    mb_cmd = $F4
    GOSUB fd_plain
    IF fn_ok = 0 THEN RETURN
    #cp_dst = FS_LIST : #cp_n = 191 : GOSUB rx_copy
    nf_ok = 0
    FOR nf_i = 0 TO 5
        #sa = FS_LIST + nf_i * 32 : #sb = VARPTR str_nf_sd_name(0) : #sn = 3 : GOSUB str_eq
        IF t_same THEN
            IF nf_ok = 0 THEN
                nf_sd = nf_i
                nf_ok = 1
            END IF
        END IF
    NEXT nf_i
    IF nf_ok = 0 THEN
        ' hosts 6 and 7 lie past the 191 bytes kept above
        mb_cmd = $F4
        GOSUB fd_plain
        FOR nf_i = 6 TO 7
            #sa = FN_RX + nf_i * 32 : #sb = VARPTR str_nf_sd_name(0) : #sn = 3 : GOSUB str_eq
            IF t_same THEN
                IF nf_ok = 0 THEN
                    nf_sd = nf_i
                    nf_ok = 1
                END IF
            END IF
        NEXT nf_i
    END IF
END

' fuji_open_directory_filter(nf_sd, path #x4/#x5, filter #x6/#x7 or 0 len)
nf_open_filter: PROCEDURE
    mb_cmd = $F7 : fz_n = 1 : fz_a = nf_sd
    GOSUB fz_set
    #cp_n = #x5
    #fn_src = #x4 : GOSUB tx_put
    POKE FN_TX + #x5, 0
    IF #x7 > 0 THEN
        FOR #fn_i = 0 TO #x7 - 1
            POKE FN_TX + #x5 + 1 + #fn_i, PEEK(#x6 + #fn_i) AND 255
        NEXT #fn_i
        POKE FN_TX + #x5 + 1 + #x7, 0
        #fn_t = #x5 + #x7 + 2
    ELSE
        POKE FN_TX + #x5 + 1, 0
        #fn_t = #x5 + 2
    END IF
    FOR #fn_i = #fn_t TO 255
        POKE FN_TX + #fn_i, 0
    NEXT #fn_i
    #fn_txlen = 256
    GOSUB fz_go
END

' fs_sd_listed: read the open directory's names; bit 1 = #x2/#x3 name seen,
' bit 2 = the second (ROM names at #x4/#x5 and #x6/#x7) -> nf_seen; closes
nf_sd_listed: PROCEDURE
    nf_seen = 0
    wd_go = 1
    nf_i = 0
    WHILE wd_go
        #cp_dst = FS_LIST : #cp_n = 49 : GOSUB buf_zero
        mb_cmd = $F6 : fz_n = 2 : fz_a = 49 : fz_b = 0
        GOSUB fz_set
        GOSUB fz_go
        IF fn_ok = 0 THEN
            wd_go = 0
        ELSE
            #cp_dst = FS_LIST : #cp_n = 49 : GOSUB rx_copy
            IF (PEEK(FS_LIST) AND 255) = $7F THEN
                wd_go = 0
            ELSE
                #sa = FS_LIST : #sb = #x4 : #sn = #x5 + 1 : GOSUB str_eq
                IF t_same THEN nf_seen = nf_seen OR 1
                #sa = FS_LIST : #sb = #x6 : #sn = #x7 + 1 : GOSUB str_eq
                IF t_same THEN nf_seen = nf_seen OR 2
            END IF
        END IF
        nf_i = nf_i + 1
        IF nf_i >= 32 THEN wd_go = 0
    WEND
    mb_cmd = $F5
    GOSUB fd_plain
END

' fs_sd_write: open WRITE, write the #x6/#x7 data, close -> nf_ok
nf_sd_write: PROCEDURE
    nf_ok = 0
    nw_mode = 8 : nw_trans = 0
    GOSUB net_open
    IF fn_ok = 0 THEN RETURN
    #fn_src = #x6 : #cp_n = #x7 : GOSUB net_write
    nf_ok = fn_ok
    GOSUB net_close
END

nf_filter_cleanup: PROCEDURE
    #fn_src = VARPTR str_nf_fone(0) : #cs_len = SL_nf_fone : mb_cmd = $21 : GOSUB nf_cmd
    #fn_src = VARPTR str_nf_ftwo(0) : #cs_len = SL_nf_ftwo : mb_cmd = $21 : GOSUB nf_cmd
    #fn_src = VARPTR str_nf_furl(0) : #cs_len = SL_nf_furl : mb_cmd = $2B : GOSUB nf_cmd
END

test_fs_directory_filter: PROCEDURE
    SECTION "fs dir filter (SD)"
    GOSUB nf_sd_host
    mb_cmd = $F9 : fz_a = nf_sd
    GOSUB fd_a1
    CHECK "SD mounts" @642, fn_ok

    GOSUB nf_filter_cleanup
    #fn_src = VARPTR str_nf_furl(0) : #cs_len = SL_nf_furl : mb_cmd = $2A : GOSUB nf_cmd
    CHECK "fnfilter made" @646, fn_ok
    #fn_src = VARPTR str_nf_fone(0) : #cs_len = SL_nf_fone
    #x6 = VARPTR str_nf_one_d(0) : #x7 = SL_nf_one_d : GOSUB nf_sd_write
    CHECK "one.txt made" @647, nf_ok
    #fn_src = VARPTR str_nf_ftwo(0) : #cs_len = SL_nf_ftwo
    #x6 = VARPTR str_nf_two_d(0) : #x7 = SL_nf_two_d : GOSUB nf_sd_write
    CHECK "two.txt made" @648, nf_ok

    #x4 = VARPTR str_nf_fdir(0) : #x5 = SL_nf_fdir : #x6 = 0 : #x7 = 0
    GOSUB nf_open_filter
    #x0 = fn_ok
    CHECK "NULL filt open" @651, #x0
    nf_seen = 0
    IF #x0 THEN
        #x4 = VARPTR str_nf_one_f(0) : #x5 = SL_nf_one_f : #x6 = VARPTR str_nf_two_f(0) : #x7 = SL_nf_two_f
        GOSUB nf_sd_listed
    END IF
    CHECK "NULL lists both" @653, nf_seen = 3

    ' a dirty buffer after the path must not be sent as the filter
    #x4 = VARPTR str_nf_fdir(0) : #x5 = SL_nf_fdir : #x6 = 0 : #x7 = 0
    GOSUB nf_open_filter
    #x0 = fn_ok
    CHECK "empty fil open" @659, #x0
    nf_seen = 0
    IF #x0 THEN
        #x4 = VARPTR str_nf_one_f(0) : #x5 = SL_nf_one_f : #x6 = VARPTR str_nf_two_f(0) : #x7 = SL_nf_two_f
        GOSUB nf_sd_listed
    END IF
    CHECK "empty both" @661, nf_seen = 3

    #x4 = VARPTR str_nf_fdir(0) : #x5 = SL_nf_fdir : #x6 = VARPTR str_nf_filt(0) : #x7 = SL_nf_filt
    GOSUB nf_open_filter
    #x0 = fn_ok
    CHECK "one.* open" @664, #x0
    nf_seen = 0
    IF #x0 THEN
        #x4 = VARPTR str_nf_one_f(0) : #x5 = SL_nf_one_f : #x6 = VARPTR str_nf_two_f(0) : #x7 = SL_nf_two_f
        GOSUB nf_sd_listed
    END IF
    CHECK "one.* one only" @666, nf_seen = 1

    GOSUB nf_filter_cleanup
    #x4 = VARPTR str_nf_fdir(0) : #x5 = SL_nf_fdir : #x6 = 0 : #x7 = 0
    GOSUB nf_open_filter
    CHECK "fnfilter gone" @672, fn_ok = 0
    mb_cmd = $F5
    GOSUB fd_plain
END

nf_hash_cleanup: PROCEDURE
    #fn_src = VARPTR str_nf_hfile(0) : #cs_len = SL_nf_hfile : mb_cmd = $21 : GOSUB nf_cmd
    #fn_src = VARPTR str_nf_hplain(0) : #cs_len = SL_nf_hplain : mb_cmd = $21 : GOSUB nf_cmd
    #fn_src = VARPTR str_nf_hurl(0) : #cs_len = SL_nf_hurl : mb_cmd = $2B : GOSUB nf_cmd
END

' reads the file at #fn_src/#cs_len and is the 4-byte "hash" -> t_in
nf_is_hash: PROCEDURE
    GOSUB nf_get
    t_in = 0
    IF #rd_n = 4 THEN
        #sa = FS_DATA : #sb = VARPTR str_nf_hdata(0) : #sn = 5 : GOSUB str_eq
        t_in = t_same
    END IF
END

test_fs_sd_hash_name: PROCEDURE
    SECTION "fs # in SD names"
    GOSUB nf_sd_host
    mb_cmd = $F9 : fz_a = nf_sd
    GOSUB fd_a1
    CHECK "SD mounts" @711, fn_ok

    GOSUB nf_hash_cleanup
    #fn_src = VARPTR str_nf_hurl(0) : #cs_len = SL_nf_hurl : mb_cmd = $2A : GOSUB nf_cmd
    CHECK "fnhash made" @714, fn_ok
    #fn_src = VARPTR str_nf_hfile(0) : #cs_len = SL_nf_hfile
    #x6 = VARPTR str_nf_hdata(0) : #x7 = SL_nf_hdata : GOSUB nf_sd_write
    CHECK "file#1 written" @715, nf_ok
    #fn_src = VARPTR str_nf_hfile(0) : #cs_len = SL_nf_hfile
    GOSUB nf_is_hash
    CHECK "file#1 reads" @716, t_in

    #x4 = VARPTR str_nf_hdir(0) : #x5 = SL_nf_hdir : #x6 = 0 : #x7 = 0
    GOSUB nf_open_filter
    nf_seen = 0
    IF fn_ok THEN
        #x4 = VARPTR str_nf_hname(0) : #x5 = SL_nf_hname : #x6 = VARPTR str_nf_hshort(0) : #x7 = SL_nf_hshort
        GOSUB nf_sd_listed
    END IF
    CHECK "lists file#1" @722, nf_seen = 1

    #fn_src = VARPTR str_nf_hesc(0) : #cs_len = SL_nf_hesc
    GOSUB nf_is_hash
    CHECK "%23 opens same" @724, t_in

    #fn_src = VARPTR str_nf_hfile(0) : #cs_len = SL_nf_hfile : mb_cmd = $21 : GOSUB nf_cmd
    CHECK "file#1 deleted" @728, fn_ok
    #x4 = VARPTR str_nf_hdir(0) : #x5 = SL_nf_hdir : #x6 = 0 : #x7 = 0
    GOSUB nf_open_filter
    nf_seen = 3
    IF fn_ok THEN
        #x4 = VARPTR str_nf_hname(0) : #x5 = SL_nf_hname : #x6 = VARPTR str_nf_hshort(0) : #x7 = SL_nf_hshort
        GOSUB nf_sd_listed
    END IF
    CHECK "file#1 gone" @731, nf_seen = 0
    GOSUB nf_hash_cleanup
END

test_fs_lock_unlock: PROCEDURE
    SECTION "fs lock/unlock"
    #x4 = VARPTR str_nf_alpha_p(0) : #x5 = SL_nf_alpha_p : GOSUB nf_path
    mb_cmd = $23 : GOSUB nf_cmd
    CHECK "lock accepted" @753, fn_ok
    #x4 = VARPTR str_nf_alpha_p(0) : #x5 = SL_nf_alpha_p : GOSUB nf_path
    #x6 = VARPTR str_nf_alpha(0) : #x7 = SL_nf_alpha : GOSUB nf_put
    CHECK "locked writes" @754, nf_ok
    #x4 = VARPTR str_nf_alpha_p(0) : #x5 = SL_nf_alpha_p : GOSUB nf_path
    mb_cmd = $24 : GOSUB nf_cmd
    CHECK "unlock accepted" @757, fn_ok
END

' open the TNFS lock file for writing and close it -> nf_ok
nf_tnfs_open_write: PROCEDURE
    nw_mode = 8 : nw_trans = 0
    #fn_src = VARPTR str_nf_lock_f(0) : #cs_len = SL_nf_lock_f
    GOSUB net_open
    nf_ok = fn_ok
    GOSUB net_close
END

test_fs_tnfs_lock_unlock: PROCEDURE
    SECTION "fs TNFS lock"
    GOSUB nf_tnfs_open_write
    CHECK "test file made" @796, nf_ok
    #fn_src = VARPTR str_nf_lock_f(0) : #cs_len = SL_nf_lock_f
    mb_cmd = $23 : GOSUB nf_cmd
    CHECK "lock ok" @799, fn_ok
    GOSUB nf_tnfs_open_write
    CHECK "locked no write" @800, nf_ok = 0
    #fn_src = VARPTR str_nf_lock_f(0) : #cs_len = SL_nf_lock_f
    mb_cmd = $24 : GOSUB nf_cmd
    CHECK "unlock ok" @803, fn_ok
    GOSUB nf_tnfs_open_write
    CHECK "unlocked writes" @804, nf_ok
    #fn_src = VARPTR str_nf_lock_f(0) : #cs_len = SL_nf_lock_f
    mb_cmd = $21 : GOSUB nf_cmd
    CHECK "test file gone" @807, fn_ok
END

test_fs_ftp_listing: PROCEDURE
    SECTION "FTP listing"
    nw_mode = 6 : nw_trans = $83
    #fn_src = VARPTR str_nf_ftp_root(0) : #cs_len = SL_nf_ftp_root
    GOSUB nf_list
    CHECK "FTP list data" @827, nf_cnt > 0
    #x6 = VARPTR str_nf_welcome(0) : #x7 = SL_nf_welcome : GOSUB nf_listed
    CHECK "last kept" @831, t_in
    #x6 = VARPTR str_nf_robots(0) : #x7 = SL_nf_robots : GOSUB nf_listed
    CHECK "plain listed" @832, t_in
    #x6 = VARPTR str_nf_bpoint(0) : #x7 = SL_nf_bpoint : GOSUB nf_listed
    CHECK "symlink name" @834, t_in
    #x6 = VARPTR str_nf_qqq(0) : #x7 = SL_nf_qqq : GOSUB nf_listed
    CHECK "no ??? entry" @836, t_in = 0
    nw_mode = 6 : nw_trans = $83
    #fn_src = VARPTR str_nf_ftp_miss(0) : #cs_len = SL_nf_ftp_miss
    GOSUB net_open
    CHECK "missing dir bad" @842, fn_ok = 0
    GOSUB net_close
END

test_ftp_crunched_name: PROCEDURE
    SECTION "FTP crunched name"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_nf_ftp_crunch(0) : #cs_len = SL_nf_ftp_crunch
    GOSUB net_open
    nf_ok = fn_ok
    CHECK "FTP RESULTS" @39, nf_ok
    IF nf_ok THEN
        #x1 = 0
        nw_conn = 1
        #nw_avail = 0
        wd_go = 1
        WHILE wd_go
            GOSUB net_status
            #x1 = #x1 + 1
            IF fn_ok = 0 THEN wd_go = 0
            IF #nw_avail <> 0 THEN wd_go = 0
            IF nw_conn = 0 THEN wd_go = 0
            IF #x1 >= 32 THEN wd_go = 0
        WEND
        #x2 = #nw_avail
        IF #x2 > 512 THEN #x2 = 512
        #rd_n = 0
        IF #x2 > 0 THEN
            #cp_dst = NB_BUF : #cp_n = #x2 : GOSUB net_read
        END IF
        CHECK "FTP read data" @53, #rd_n > 0
        GOSUB net_close
    END IF
END

test_netfile: PROCEDURE
    GOSUB test_fs_prefix : IF t_abort THEN RETURN
    GOSUB test_fs_make_test_dir : IF t_abort THEN RETURN
    GOSUB test_fs_create_files : IF t_abort THEN RETURN
    GOSUB test_fs_directory_modes : IF t_abort THEN RETURN
    GOSUB test_fs_long_listing : IF t_abort THEN RETURN
    GOSUB test_fs_read_files : IF t_abort THEN RETURN
    GOSUB test_fs_put_body_writes : IF t_abort THEN RETURN
    GOSUB test_fs_rename_delete : IF t_abort THEN RETURN
    GOSUB test_fs_dir_lifecycle : IF t_abort THEN RETURN
    GOSUB test_fs_directory_filter : IF t_abort THEN RETURN
    GOSUB test_fs_sd_hash_name : IF t_abort THEN RETURN
    GOSUB test_fs_lock_unlock : IF t_abort THEN RETURN
    GOSUB test_fs_tnfs_lock_unlock : IF t_abort THEN RETURN
    GOSUB test_fs_ftp_listing : IF t_abort THEN RETURN
    GOSUB test_ftp_crunched_name
END
