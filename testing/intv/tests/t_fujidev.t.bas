' fujidev: tests/fujidev.c and tests/crypto.c
    CONST HS_A  = $9300         ' 256: host slots
    CONST HS_B  = $9400         ' 256
    CONST DS_A  = $9500         ' 304: 8 device slots of 38 bytes
    CONST DS_B  = $9630         ' 304, directly after DS_A
    CONST FD_A  = $9780         ' 96: filename / prefix
    CONST FD_B  = $97E0         ' 96
    CONST FD_C  = $9840         ' 64: directory entry
    CONST G1    = $9880         ' 40: GUIDs
    CONST G2    = $98B0
    CONST B64E  = $98E0         ' 48
    CONST B64D  = $9910         ' 48
    CONST HASHA = $9940         ' 132
    CONST HASHC = $99D0         ' 65: single-shot SHA256
    CONST INC1  = $9A20         ' 65
    CONST INC2  = $9A70         ' 65

    DIM fd_ok, fd_ok2, fd_t, fd_mode, fd_host, fd_tm, fd_th

STR hello "Hello, FujiNet!"
STR b64out "SGVsbG8sIEZ1amlOZXQh"
STR md5out "98d6da021c1a0b081b52de3b8207a823"
STR sha1out "954382c7aa0a0d3252faf234ba4911f2aed22e39"
STR sha256out "3631e38122de15b2e5b4ae209ba1eb7c79b0e756fd9bc7149ce83da8aba6ddc6"
STR sha512out "8230a7a465c8ef21e6d89c423e5cb06f1f8b183b7928189736e00e81d269484d5a5d8a40723a157ef55d1992fd2929b213770eada5501434a592d75ad1f935fd"
STR tfile "/test_filename"
STR tprefix "/test_prefix"
STR rootdir "/"

' zero #cp_n bytes at #sa, leaving #cp_dst there
fd_reply: PROCEDURE
    #cp_dst = #sa : GOSUB buf_zero
    #cp_dst = #sa
END

test_fuji_status: PROCEDURE
    SECTION "fuji_status"
    FOR fd_t = 0 TO 3
        POKE HS_A + fd_t, $FF
    NEXT fd_t
    mb_cmd = $53 : fz_a = 0
    GOSUB fd_a1
    fd_ok = fn_ok
    #cp_dst = HS_A : #cp_n = 4 : GOSUB rx_copy
    CHECK "status true" @33, fd_ok
    #x0 = 0
    FOR fd_t = 0 TO 3
        IF (PEEK(HS_A + fd_t) AND 255) <> $FF THEN #x0 = 1
    NEXT fd_t
    CHECK "status wrote" @35, #x0
END

test_fuji_adapter_config: PROCEDURE
    SECTION "adapter config"
    #cp_dst = HS_A : #cp_n = 240 : GOSUB buf_zero
    mb_cmd = $E8
    GOSUB fd_plain
    CHECK "adapter cfg ok" @53, fn_ok
    #cp_dst = HS_A : #cp_n = 140 : GOSUB rx_copy
    #x0 = (PEEK(HS_A + 97) AND 255) + (PEEK(HS_A + 98) AND 255) + (PEEK(HS_A + 99) AND 255) + (PEEK(HS_A + 100) AND 255)
    CHECK "local ip set" @57, #x0 <> 0
    #x0 = 0
    FOR fd_t = 113 TO 118
        #x0 = #x0 + (PEEK(HS_A + fd_t) AND 255)
    NEXT fd_t
    CHECK "mac set" @61, #x0 <> 0
    CHECK "version set" @63, (PEEK(HS_A + 125) AND 255) <> 0

    #cp_dst = HS_A : #cp_n = 240 : GOSUB buf_zero
    mb_cmd = $C4
    GOSUB fd_plain
    CHECK "ext cfg ok" @74, fn_ok
    #cp_dst = HS_A : #cp_n = 240 : GOSUB rx_copy
    CHECK "ext sLocalIP" @75, (PEEK(HS_A + 140) AND 255) <> 0
    CHECK "ext sMac" @76, (PEEK(HS_A + 204) AND 255) <> 0
END

test_fuji_host_device_slots: PROCEDURE
    SECTION "host/device slots"
    #cp_dst = HS_A : #cp_n = 256 : GOSUB buf_zero
    mb_cmd = $F4
    GOSUB fd_plain
    fd_ok = fn_ok
    #cp_dst = HS_A : #cp_n = 256 : GOSUB rx_copy
    CHECK "get hosts ok" @95, fd_ok

    mb_cmd = $F3 : fz_n = 0
    GOSUB fz_set
    #fn_src = HS_A : #cp_n = 256 : GOSUB tx_put
    GOSUB fz_go
    CHECK "put hosts ok" @103, fn_ok

    #cp_dst = HS_B : #cp_n = 256 : GOSUB buf_zero
    mb_cmd = $F4
    GOSUB fd_plain
    fd_ok = fn_ok
    #cp_dst = HS_B : #cp_n = 256 : GOSUB rx_copy
    CHECK "reread hosts ok" @107, fd_ok
    #sa = HS_A : #sb = HS_B : #sn = 256
    GOSUB str_eq
    CHECK "hosts unchanged" @108, t_same

    ' sentinel after the array: a reply longer than 8 slots would show here
    #cp_dst = DS_A : #cp_n = 304 : GOSUB buf_zero
    #cp_dst = DS_B : #cp_n = 304
    FOR #fn_i = 1 TO 304
        POKE DS_B + #fn_i - 1, $A5
    NEXT #fn_i
    mb_cmd = $F2
    GOSUB fd_plain
    fd_ok = fn_ok
    #cp_dst = DS_A : #cp_n = 512 : GOSUB rx_copy
    CHECK "get devslots ok" @119, fd_ok
    #x0 = 1
    FOR #fn_i = 1 TO 304
        IF (PEEK(DS_B + #fn_i - 1) AND 255) <> $A5 THEN #x0 = 0
    NEXT #fn_i
    CHECK "no extra devs" @131, #x0

    ' 8 slots are 304 bytes and the mailbox carries 256: nothing to send.
    SKIPCHK "put devslots ok" @140

    #cp_dst = DS_B : #cp_n = 304 : GOSUB buf_zero
    mb_cmd = $F2
    GOSUB fd_plain
    fd_ok = fn_ok
    #cp_dst = DS_B : #cp_n = 304 : GOSUB rx_copy
    CHECK "reread devs ok" @144, fd_ok
    #sa = DS_A : #sb = DS_B : #sn = 304
    GOSUB str_eq
    CHECK "devs unchanged" @145, t_same

    mb_cmd = $F9 : fz_a = $FF
    GOSUB fd_a1
    CHECK "bad mount fails" @153, fn_ok = 0
END

test_fuji_host_slots_oversized: PROCEDURE
    SECTION "oversized write"
    #cp_dst = HS_A : #cp_n = 256 : GOSUB buf_zero
    mb_cmd = $F4
    GOSUB fd_plain
    fd_ok = fn_ok
    #cp_dst = HS_A : #cp_n = 256 : GOSUB rx_copy
    CHECK "get hosts ok" @171, fd_ok

    ' The mailbox carries 256 bytes, so the 64 trailing zero bytes of the C
    ' test cannot be sent; the table itself goes back.
    mb_cmd = $F3 : fz_n = 0
    GOSUB fz_set
    #fn_src = HS_A : #cp_n = 256 : GOSUB tx_put
    GOSUB fz_go

    mb_cmd = $E8
    GOSUB fd_plain
    CHECK "still answers" @178, fn_ok

    #cp_dst = HS_B : #cp_n = 256 : GOSUB buf_zero
    mb_cmd = $F4
    GOSUB fd_plain
    fd_ok = fn_ok
    #cp_dst = HS_B : #cp_n = 256 : GOSUB rx_copy
    #sa = HS_A : #sb = HS_B : #sn = 256
    GOSUB str_eq
    #x0 = 0
    IF fd_ok THEN #x0 = t_same
    CHECK "hosts unchanged" @183, #x0
END

' Get device slot 0's host and mode: fd_host, fd_mode (fuji_get_device_slots)
fd_slot0: PROCEDURE
    #cp_dst = DS_A : #cp_n = 304 : GOSUB buf_zero
    mb_cmd = $F2
    GOSUB fd_plain
    fd_ok = fn_ok
    #cp_dst = DS_A : #cp_n = 304 : GOSUB rx_copy
    #fn_t = PEEK(DS_A) AND 255
    fd_host = #fn_t
    #fn_t = PEEK(DS_A + 1) AND 255
    fd_mode = #fn_t
END

' fuji_get_device_filename(0) -> #cp_dst (96 bytes)
fd_get_fname: PROCEDURE
    #sa = #cp_dst : #cp_n = 96
    GOSUB fd_reply
    mb_cmd = $DA : fz_a = 0
    GOSUB fd_a1
    IF fn_ok THEN
        #cp_n = 95 : GOSUB rx_to_buf
    END IF
END

' fuji_set_device_filename(fd_mode, fd_host, slot 0, #fn_src/#cs_len)
fd_set_fname: PROCEDURE
    mb_cmd = $E2 : fz_n = 3 : fz_a = 0 : fz_b = fd_host : fz_c = fd_mode
    GOSUB fz_set
    GOSUB fz_pad
    GOSUB fz_go
END

test_fuji_device_filename: PROCEDURE
    SECTION "device filename"
    GOSUB fd_slot0
    fd_tm = fd_mode : fd_th = fd_host       ' original mode and host

    #cp_dst = FD_A
    GOSUB fd_get_fname
    CHECK "get fname ok" @215, fn_ok

    fd_mode = 1 : fd_host = 0
    #fn_src = VARPTR str_tfile(0) : #cs_len = SL_tfile
    GOSUB fd_set_fname
    CHECK "set fname ok" @222, fn_ok

    #cp_dst = FD_B
    GOSUB fd_get_fname
    CHECK "reread fname ok" @225, fn_ok
    #sa = FD_B : #sb = VARPTR str_tfile(0) : #sn = SL_tfile + 1
    GOSUB str_eq
    CHECK "fname matches" @226, t_same

    GOSUB fd_slot0
    fd_ok2 = fd_ok
    #x1 = fd_host : #x2 = fd_mode           ' what the firmware reports
    fd_t = fd_host
    fd_mode = #x2
    #fn_src = VARPTR str_tfile(0) : #cs_len = SL_tfile
    GOSUB fd_set_fname
    GOSUB fd_slot0
    fd_ok2 = fd_ok2 AND fd_ok
    #x0 = 0
    IF fd_ok2 THEN
        IF fd_host = fd_t THEN #x0 = 1
    END IF
    CHECK "mode keeps host" @238, #x0

    ' restore the original
    fd_mode = fd_tm : fd_host = fd_th
    #fn_src = FD_A
    GOSUB str_len
    #cs_len = #cp_n
    GOSUB fd_set_fname
    #cp_dst = FD_B
    GOSUB fd_get_fname
    CHECK "orig fname ok" @244, fn_ok
    #sa = FD_B : #sb = FD_A : #sn = #cs_len + 1
    GOSUB str_eq
    CHECK "orig matches" @245, t_same
    GOSUB fd_slot0
    #x0 = 0
    IF fd_ok THEN
        IF fd_host = fd_th THEN #x0 = 1
    END IF
    CHECK "host restored" @248, #x0
END

' fuji_get_host_prefix(0) -> #cp_dst
fd_get_prefix: PROCEDURE
    #sa = #cp_dst : #cp_n = 96
    GOSUB fd_reply
    mb_cmd = $E0 : fz_a = 0
    GOSUB fd_a1
    IF fn_ok THEN
        #cp_n = 95 : GOSUB rx_to_buf
    END IF
END

' fuji_set_host_prefix(0, #fn_src/#cs_len)
fd_set_prefix: PROCEDURE
    mb_cmd = $E1 : fz_n = 1 : fz_a = 0
    GOSUB fz_set
    GOSUB fz_pad
    GOSUB fz_go
END

test_fuji_host_prefix: PROCEDURE
    SECTION "host prefix"
    #cp_dst = FD_A
    GOSUB fd_get_prefix
    CHECK "get prefix ok" @267, fn_ok

    #fn_src = VARPTR str_tprefix(0) : #cs_len = SL_tprefix
    GOSUB fd_set_prefix
    CHECK "set prefix ok" @274, fn_ok

    #cp_dst = FD_B
    GOSUB fd_get_prefix
    CHECK "reread prefix" @277, fn_ok
    #sa = FD_B : #sb = VARPTR str_tprefix(0) : #sn = SL_tprefix + 1
    GOSUB str_eq
    CHECK "prefix matches" @278, t_same

    #fn_src = FD_A
    GOSUB str_len
    #cs_len = #cp_n
    GOSUB fd_set_prefix
END

' open directory: path at #fn_src/#cs_len then an empty filter, 256 bytes
fd_open_dir: PROCEDURE
    mb_cmd = $F7 : fz_n = 1 : fz_a = 0
    GOSUB fz_set
    GOSUB fz_pad
    GOSUB fz_go
END

test_fuji_directory_ops: PROCEDURE
    SECTION "directory ops"
    mb_cmd = $F9 : fz_a = 0
    GOSUB fd_a1
    CHECK "mount host 0" @299, fn_ok

    #fn_src = VARPTR str_rootdir(0) : #cs_len = SL_rootdir
    GOSUB fd_open_dir
    CHECK "open dir /" @313, fn_ok

    #sa = FD_C : #cp_n = 64
    GOSUB fd_reply
    mb_cmd = $F6 : fz_n = 2 : fz_a = 63 : fz_b = 0
    GOSUB fz_set
    GOSUB fz_go
    fd_ok = fn_ok
    #cp_dst = FD_C : #cp_n = 63 : GOSUB rx_copy
    CHECK "read 1st entry" @320, fd_ok

    #x6 = 0
    mb_cmd = $E5
    GOSUB fd_plain
    CHECK "get dir pos" @328, fn_ok

    #fz_w = 0
    mb_cmd = $E4
    GOSUB fz_b12
    GOSUB fz_go
    CHECK "set dir pos 0" @335, fn_ok

    mb_cmd = $E5
    GOSUB fd_plain
    fd_ok = fn_ok
    #x6 = $FFFF
    IF fd_ok THEN #x6 = (PEEK(FN_RX) AND 255) + (PEEK(FN_RX + 1) AND 255) * 256
    CHECK "pos is 0" @339, #x6 = 0

    mb_cmd = $F5
    GOSUB fd_plain
    CHECK "close dir" @345, fn_ok

    #fn_src = VARPTR str_rootdir(0) : #cs_len = SL_rootdir
    GOSUB fd_open_dir
    CHECK "open dir filter" @351, fn_ok

    mb_cmd = $F5
    GOSUB fd_plain
    CHECK "close dir 2" @354, fn_ok
END

test_fuji_guid: PROCEDURE
    SECTION "generate guid"
    #cp_dst = G1 : #cp_n = 40 : GOSUB buf_zero
    #cp_dst = G2 : GOSUB buf_zero
    mb_cmd = $BB
    GOSUB fd_plain
    fd_ok = fn_ok
    #cp_dst = G1 : #cp_n = 37 : GOSUB rx_copy
    GOSUB fd_plain
    fd_ok2 = fn_ok
    #cp_dst = G2 : #cp_n = 37 : GOSUB rx_copy
    CHECK "guid 1 ok" @209, fd_ok
    CHECK "guid 2 ok" @210, fd_ok2
    #fn_src = G1 : GOSUB str_len
    CHECK "guid1 len 36" @211, #cp_n = 36
    #fn_src = G2 : GOSUB str_len
    CHECK "guid2 len 36" @212, #cp_n = 36
    #sa = G1 : #sb = G2 : #sn = 37
    GOSUB str_eq
    CHECK "guids differ" @213, t_same = 0
END

' base64: reply of the *_length command, 4 bytes little-endian; low 16 bits
fd_len32: PROCEDURE
    #x7 = 0
    IF fn_ok THEN #x7 = (PEEK(FN_RX) AND 255) + (PEEK(FN_RX + 1) AND 255) * 256
END

' Send #fn_src/#cs_len as the payload of base64 input mb_cmd
fd_b64_in: PROCEDURE
    #fz_w = #cs_len
    GOSUB fz_b12
    #cp_n = #cs_len
    GOSUB tx_put
    GOSUB fz_go
END

' base64 output: #x7 bytes to #cp_dst
fd_b64_out: PROCEDURE
    #fz_w = #x7
    GOSUB fz_b12
    GOSUB fz_go
    #cp_n = #x7
    IF fn_ok THEN GOSUB rx_copy
END

test_fuji_base64: PROCEDURE
    SECTION "base64"
    ' drain anything a previous run left in either pipeline
    mb_cmd = $CE
    GOSUB fd_plain
    GOSUB fd_len32
    WHILE #x7 > 0
        #cp_dst = HS_A : mb_cmd = $CD
        IF #x7 > 200 THEN #x7 = 200
        GOSUB fd_b64_out
        mb_cmd = $CE
        GOSUB fd_plain
        GOSUB fd_len32
    WEND
    mb_cmd = $CA
    GOSUB fd_plain
    GOSUB fd_len32
    WHILE #x7 > 0
        #cp_dst = HS_A : mb_cmd = $C9
        IF #x7 > 200 THEN #x7 = 200
        GOSUB fd_b64_out
        mb_cmd = $CA
        GOSUB fd_plain
        GOSUB fd_len32
    WEND

    mb_cmd = $D0
    #fn_src = VARPTR str_hello(0) : #cs_len = SL_hello
    GOSUB fd_b64_in
    CHECK "enc input ok" @122, fn_ok
    mb_cmd = $CF
    GOSUB fd_plain
    CHECK "enc compute ok" @128, fn_ok
    mb_cmd = $CE
    GOSUB fd_plain
    fd_ok = fn_ok
    GOSUB fd_len32
    #x3 = #x7                          ' enc_len
    CHECK "enc length ok" @135, fd_ok
    CHECK "enc len > 0" @136, #x3 > 0
    CHECK "enc len fits" @137, #x3 < 48

    #cp_dst = B64E : #cp_n = 48 : GOSUB buf_zero
    #cp_dst = B64E : mb_cmd = $CD
    GOSUB fd_b64_out
    CHECK "enc output ok" @143, fn_ok
    POKE B64E + #x3, 0
    ' strip trailing 9B / 0A / 0D, as strip_trailing_newlines does
    #x4 = #x3
    #x5 = 1
    WHILE #x5
        #x5 = 0
        IF #x4 > 0 THEN
            #fn_t = PEEK(B64E + #x4 - 1) AND 255
            IF #fn_t = $9B THEN #x5 = 1
            IF #fn_t = $0A THEN #x5 = 1
            IF #fn_t = $0D THEN #x5 = 1
            IF #x5 THEN #x4 = #x4 - 1
        END IF
    WEND
    POKE B64E + #x4, 0
    #sa = B64E : #sb = VARPTR str_b64out(0) : #sn = SL_b64out + 1
    GOSUB str_eq
    CHECK "b64 matches" @153, t_same

    mb_cmd = $CC
    #fn_src = B64E : #cs_len = #x3
    GOSUB fd_b64_in
    CHECK "dec input ok" @160, fn_ok
    mb_cmd = $CB
    GOSUB fd_plain
    CHECK "dec compute ok" @166, fn_ok
    mb_cmd = $CA
    GOSUB fd_plain
    fd_ok = fn_ok
    GOSUB fd_len32
    CHECK "dec length ok" @173, fd_ok
    CHECK "dec len fits" @174, #x7 < 32
    #cp_dst = B64D : #cp_n = 48 : GOSUB buf_zero
    #cp_dst = B64D : mb_cmd = $C9
    GOSUB fd_b64_out
    CHECK "dec output ok" @180, fn_ok
    POKE B64D + #x7, 0
    #sa = B64D : #sb = VARPTR str_hello(0) : #sn = SL_hello + 1
    GOSUB str_eq
    CHECK "dec matches" @186, t_same
END

' fuji_hash_data(type fz_a, hex): hex digest to #cp_dst, NUL-terminated
fd_hash_data: PROCEDURE
    mb_cmd = $C2
    GOSUB fd_plain
    IF fn_ok = 0 THEN RETURN
    mb_cmd = $C8
    #fn_src = VARPTR str_hello(0) : #cs_len = SL_hello
    GOSUB fd_b64_in
    IF fn_ok = 0 THEN RETURN
    mb_cmd = $C7 : fz_n = 1 : fz_a = fd_t
    GOSUB fz_set
    GOSUB fz_go
    IF fn_ok = 0 THEN RETURN
    GOSUB fd_hash_out
    fn_ok = 1
END

' hash_length(hex) then hash_output(hex): digest to #cp_dst + NUL
fd_hash_out: PROCEDURE
    mb_cmd = $C6 : fz_n = 1 : fz_a = 1
    GOSUB fz_set
    GOSUB fz_go
    #x7 = PEEK(FN_RX) AND 255
    mb_cmd = $C5 : fz_n = 1 : fz_a = 1
    GOSUB fz_set
    GOSUB fz_go
    #cp_n = #x7
    GOSUB rx_to_buf
END

' fuji_hash_size(type, hex)
hs_tab:
    DATA 32, 40, 64, 128

test_fuji_hashing: PROCEDURE
    SECTION "hash single-shot"
    #cp_dst = HASHA : #cp_n = 132 : GOSUB buf_zero

    fd_t = 0 : #cp_dst = HASHA : #sa = HASHA
    GOSUB fd_hash_data
    fd_ok = fn_ok
    #fn_src = HASHA : GOSUB str_len
    #x0 = #cp_n
    #sa = HASHA : #sb = VARPTR str_md5out(0) : #sn = SL_md5out + 1
    GOSUB str_eq
    CHECK "md5 size 32" @239, (PEEK(VARPTR hs_tab(0) + 0) AND 255) = 32
    CHECK "md5 ok" @241, fd_ok
    CHECK "md5 len 32" @244, #x0 = 32
    CHECK "md5 matches" @245, t_same

    #cp_dst = HASHA : #cp_n = 132 : GOSUB buf_zero
    fd_t = 1 : #cp_dst = HASHA
    GOSUB fd_hash_data
    fd_ok = fn_ok
    #fn_src = HASHA : GOSUB str_len
    #x0 = #cp_n
    #sa = HASHA : #sb = VARPTR str_sha1out(0) : #sn = SL_sha1out + 1
    GOSUB str_eq
    CHECK "sha1 size 40" @248, (PEEK(VARPTR hs_tab(0) + 1) AND 255) = 40
    CHECK "sha1 ok" @250, fd_ok
    CHECK "sha1 len 40" @253, #x0 = 40
    CHECK "sha1 matches" @254, t_same

    #cp_dst = HASHA : #cp_n = 132 : GOSUB buf_zero
    fd_t = 2 : #cp_dst = HASHA
    GOSUB fd_hash_data
    fd_ok = fn_ok
    #fn_src = HASHA : GOSUB str_len
    #x0 = #cp_n
    #sa = HASHA : #sb = VARPTR str_sha256out(0) : #sn = SL_sha256out + 1
    GOSUB str_eq
    CHECK "sha256 size 64" @257, (PEEK(VARPTR hs_tab(0) + 2) AND 255) = 64
    CHECK "sha256 ok" @259, fd_ok
    CHECK "sha256 len 64" @262, #x0 = 64
    CHECK "sha256 matches" @263, t_same
    FOR #fn_i = 0 TO 64
        POKE HASHC + #fn_i, PEEK(HASHA + #fn_i) AND 255
    NEXT #fn_i

    #cp_dst = HASHA : #cp_n = 132 : GOSUB buf_zero
    fd_t = 3 : #cp_dst = HASHA
    GOSUB fd_hash_data
    fd_ok = fn_ok
    #fn_src = HASHA : GOSUB str_len
    #x0 = #cp_n
    #sa = HASHA : #sb = VARPTR str_sha512out(0) : #sn = SL_sha512out + 1
    GOSUB str_eq
    CHECK "sha512 size 128" @266, (PEEK(VARPTR hs_tab(0) + 3) AND 255) = 128
    CHECK "sha512 ok" @268, fd_ok
    CHECK "sha512 len 128" @271, #x0 = 128
    CHECK "sha512 matches" @272, t_same

    SECTION "hash incremental"
    mb_cmd = $C2
    GOSUB fd_plain
    CHECK "hash clear ok" @281, fn_ok

    mb_cmd = $C8
    #fn_src = VARPTR str_hello(0) : #cs_len = SL_hello
    GOSUB fd_b64_in
    CHECK "hash add ok" @288, fn_ok

    #cp_dst = INC1 : #cp_n = 65 : GOSUB buf_zero
    mb_cmd = $C3 : fz_n = 1 : fz_a = 2
    GOSUB fz_set
    GOSUB fz_go
    fd_ok = fn_ok
    #cp_dst = INC1
    GOSUB fd_hash_out
    CHECK "calc keep ok" @296, fd_ok
    #fn_src = INC1 : GOSUB str_len
    CHECK "inc1 len 64" @297, #cp_n = 64

    #cp_dst = INC2 : #cp_n = 65 : GOSUB buf_zero
    mb_cmd = $C7 : fz_n = 1 : fz_a = 2
    GOSUB fz_set
    GOSUB fz_go
    fd_ok = fn_ok
    #cp_dst = INC2
    GOSUB fd_hash_out
    mb_cmd = $C2
    GOSUB fd_plain
    CHECK "calc discard ok" @301, fd_ok
    #sa = INC1 : #sb = INC2 : #sn = 64
    GOSUB str_eq
    CHECK "second matches" @302, t_same
    #sa = INC1 : #sb = HASHC : #sn = 64
    GOSUB str_eq
    CHECK "matches 1-shot" @304, t_same
END

test_fujidev: PROCEDURE
    GOSUB test_fuji_status : IF t_abort THEN RETURN
    GOSUB test_fuji_adapter_config : IF t_abort THEN RETURN
    GOSUB test_fuji_host_device_slots : IF t_abort THEN RETURN
    GOSUB test_fuji_host_slots_oversized : IF t_abort THEN RETURN
    GOSUB test_fuji_device_filename : IF t_abort THEN RETURN
    GOSUB test_fuji_host_prefix : IF t_abort THEN RETURN
    GOSUB test_fuji_directory_ops : IF t_abort THEN RETURN
    GOSUB test_fuji_guid : IF t_abort THEN RETURN
    GOSUB test_fuji_base64 : IF t_abort THEN RETURN
    GOSUB test_fuji_hashing
END
