' qrcode: tests/qrcode.c (the expected byte arrays come from that file)
    CONST QL_bin = 57
qr_bin:
    DATA 21, 127, 213, 63, 136, 9, 118, 45, 221, 174, 169, 219
    DATA 149, 117, 131, 140, 224, 95, 245, 7, 64, 0, 79, 47
    DATA 151, 131, 213, 170, 21, 226, 94, 133, 149, 212, 15, 1
    DATA 242, 234, 31, 66, 8, 18, 245, 93, 146, 175, 171, 204
    DATA 117, 101, 147, 160, 242, 248, 23, 79, 0
    CONST QL_bmp = 64
qr_bmp:
    DATA 21, 254, 171, 248, 130, 50, 8, 186, 210, 232, 186, 202
    DATA 232, 186, 154, 232, 130, 98, 8, 254, 171, 248, 0, 16
    DATA 0, 242, 244, 232, 56, 53, 104, 86, 161, 24, 189, 80
    DATA 208, 146, 191, 8, 0, 158, 168, 254, 16, 128, 130, 69
    DATA 120, 186, 73, 240, 186, 166, 112, 186, 155, 32, 130, 167
    DATA 136, 254, 143, 32
    CONST QL_ata = 133
qr_ata:
    DATA 21, 137, 149, 149, 25, 12, 139, 12, 137, 149, 149, 25
    DATA 155, 25, 160, 25, 25, 160, 11, 15, 25, 160, 25, 25
    DATA 155, 25, 149, 12, 25, 7, 6, 12, 25, 149, 12, 25
    DATA 155, 149, 149, 149, 12, 12, 7, 12, 149, 149, 149, 12
    DATA 155, 149, 160, 15, 12, 149, 160, 153, 9, 143, 25, 25
    DATA 155, 6, 140, 140, 7, 7, 7, 32, 11, 21, 153, 12
    DATA 155, 12, 11, 32, 12, 25, 143, 160, 137, 15, 15, 25
    DATA 155, 137, 149, 149, 25, 9, 11, 9, 9, 7, 21, 15
    DATA 155, 25, 160, 25, 25, 6, 15, 7, 6, 143, 160, 32
    DATA 155, 25, 149, 12, 25, 25, 6, 7, 160, 15, 12, 15
    DATA 155, 149, 149, 149, 12, 12, 32, 149, 149, 32, 12, 32
    DATA 155
    CONST QL_pet = 196
qr_pet:
    DATA 13, 32, 18, 172, 162, 162, 146, 161, 190, 18, 188, 146
    DATA 190, 18, 172, 162, 162, 146, 161, 13, 32, 161, 18, 32
    DATA 146, 161, 161, 18, 32, 146, 188, 187, 161, 18, 32, 146
    DATA 161, 161, 13, 32, 161, 18, 162, 146, 190, 161, 191, 18
    DATA 191, 146, 190, 161, 18, 162, 146, 190, 161, 13, 32, 18
    DATA 162, 162, 162, 146, 190, 190, 191, 190, 18, 162, 162, 162
    DATA 146, 190, 13, 32, 18, 162, 32, 146, 187, 190, 18, 162
    DATA 32, 161, 146, 172, 18, 187, 146, 161, 161, 13, 32, 18
    DATA 191, 190, 190, 146, 191, 191, 191, 32, 188, 162, 18, 161
    DATA 146, 190, 13, 32, 190, 188, 32, 190, 161, 18, 187, 32
    DATA 172, 146, 187, 187, 161, 13, 32, 18, 172, 162, 162, 146
    DATA 161, 172, 188, 172, 172, 191, 162, 187, 13, 32, 161, 18
    DATA 32, 146, 161, 161, 18, 191, 146, 187, 191, 18, 191, 187
    DATA 32, 146, 32, 13, 32, 161, 18, 162, 146, 190, 161, 161
    DATA 18, 191, 146, 191, 18, 32, 146, 187, 190, 187, 13, 32
    DATA 18, 162, 162, 162, 146, 190, 190, 32, 18, 162, 162, 146
    DATA 32, 190, 32, 13
    CONST QL_med = 80
qr_med:
    DATA 25, 127, 157, 253, 131, 236, 10, 118, 45, 215, 237, 146
    DATA 162, 219, 69, 78, 55, 40, 144, 224, 95, 85, 127, 0
    DATA 231, 0, 197, 98, 73, 47, 230, 216, 22, 27, 202, 205
    DATA 40, 97, 53, 44, 91, 146, 149, 146, 237, 244, 252, 90
    DATA 202, 174, 21, 219, 225, 191, 0, 198, 227, 252, 45, 84
    DATA 12, 34, 141, 220, 133, 251, 163, 75, 82, 76, 215, 221
    DATA 253, 32, 172, 3, 127, 103, 59, 1
    CONST QL_qrt = 80
qr_qrt:
    DATA 25, 127, 146, 253, 131, 240, 10, 118, 25, 215, 237, 202
    DATA 162, 219, 69, 78, 55, 232, 147, 224, 95, 85, 127, 128
    DATA 229, 0, 70, 114, 45, 60, 246, 216, 66, 39, 202, 93
    DATA 189, 97, 133, 110, 91, 82, 131, 145, 237, 24, 236, 90
    DATA 6, 147, 21, 119, 251, 191, 0, 154, 227, 252, 121, 85
    DATA 12, 242, 143, 220, 37, 253, 163, 203, 84, 76, 215, 200
    DATA 253, 160, 141, 3, 127, 124, 59, 1
    CONST QL_hig = 107
qr_hig:
    DATA 29, 127, 137, 210, 63, 8, 110, 11, 118, 41, 67, 221
    DATA 174, 200, 167, 219, 69, 243, 116, 131, 36, 153, 224, 95
    DATA 85, 245, 7, 48, 3, 0, 116, 183, 56, 18, 225, 116
    DATA 39, 91, 253, 231, 240, 21, 235, 153, 53, 157, 176, 5
    DATA 114, 181, 228, 161, 156, 113, 177, 151, 81, 203, 8, 68
    DATA 202, 196, 25, 129, 10, 81, 149, 61, 2, 127, 143, 182
    DATA 86, 25, 37, 34, 223, 0, 130, 47, 250, 159, 191, 87
    DATA 15, 58, 133, 200, 93, 85, 245, 169, 75, 35, 179, 117
    DATA 53, 122, 199, 32, 53, 102, 246, 103, 140, 200, 1
    CONST QL_v2 = 80
qr_v2:
    DATA 25, 127, 153, 253, 131, 238, 10, 118, 113, 215, 237, 34
    DATA 162, 219, 245, 78, 55, 40, 145, 224, 95, 85, 127, 128
    DATA 238, 0, 103, 103, 159, 111, 205, 216, 78, 33, 202, 221
    DATA 24, 97, 181, 180, 91, 146, 49, 146, 237, 157, 253, 90
    DATA 134, 173, 21, 203, 230, 191, 0, 198, 227, 252, 49, 84
    DATA 12, 42, 141, 220, 165, 251, 163, 139, 82, 76, 215, 221
    DATA 253, 160, 175, 3, 127, 103, 59, 1

    CONST QR_OUT1 = $9300       ' 200 bytes: the low-level output
    CONST QR_OUT2 = $9400       ' 64 bytes: qrcode_create's output
    DIM qr_v, qr_e, qr_m, qr_s, qr_ok

STR qr_in "Hello, FujiNet!"
STR qr_url "https://fujinet.io/"

' fuji_qrcode_input: the #x4/#x5 bytes
qr_input: PROCEDURE
    mb_cmd = $BC
    #fz_w = #x5
    GOSUB fz_b12
    #fn_src = #x4 : #cp_n = #x5 : GOSUB tx_put
    GOSUB fz_go
END

' fuji_qrcode_encode(qr_v, qr_e, qr_s)
qr_encode: PROCEDURE
    mb_cmd = $BD : fz_n = 3 : fz_a = qr_v : fz_b = qr_e : fz_c = qr_s
    GOSUB fz_set
    GOSUB fz_go
END

' fuji_qrcode_length(qr_m) -> #x3 (low 16 bits), #x2 = the upper 16 bits
qr_length: PROCEDURE
    mb_cmd = $BE : fz_n = 1 : fz_a = qr_m
    GOSUB fz_set
    GOSUB fz_go
    #x3 = 0 : #x2 = 0
    IF fn_ok THEN
        #x3 = (PEEK(FN_RX) AND 255) + (PEEK(FN_RX + 1) AND 255) * 256
        #x2 = (PEEK(FN_RX + 2) AND 255) + (PEEK(FN_RX + 3) AND 255) * 256
    END IF
END

' fuji_qrcode_output: #fz_w bytes to #cp_dst
qr_output: PROCEDURE
    mb_cmd = $BF
    GOSUB fz_b12
    GOSUB fz_go
    #cp_n = #fz_w
    IF fn_ok THEN GOSUB rx_copy
END

' qr_encode_ready: input #x4/#x5, encode, length -> qr_ok, length in #x3
qr_ready: PROCEDURE
    qr_ok = 0
    GOSUB qr_input
    IF fn_ok = 0 THEN RETURN
    GOSUB qr_encode
    IF fn_ok = 0 THEN RETURN
    GOSUB qr_length
    qr_ok = fn_ok
END

' qr_check_mode: encode with qr_v/qr_e/qr_m, compare with the expected array at
' #x6, length #x7 -> qr_ok
qr_check_mode: PROCEDURE
    #x4 = VARPTR str_qr_in(0) : #x5 = SL_qr_in
    GOSUB qr_ready
    IF qr_ok = 0 THEN RETURN
    qr_ok = 0
    IF #x3 <> #x7 THEN RETURN
    #cp_dst = QR_OUT1 : #cp_n = 200 : GOSUB buf_zero
    #fz_w = #x7
    #cp_dst = QR_OUT1
    GOSUB qr_output
    IF fn_ok = 0 THEN RETURN
    #sa = QR_OUT1 : #sb = #x6 : #sn = #x7 : GOSUB str_eq
    qr_ok = t_same
END

' qr_encode_and_drain: input #x4/#x5, encode with qr_v/qr_e/qr_s, length of
' mode qr_m, then read it all in 256-byte pieces -> qr_ok
qr_drain: PROCEDURE
    qr_ok = 0
    GOSUB qr_input
    IF fn_ok = 0 THEN RETURN
    GOSUB qr_encode
    IF fn_ok = 0 THEN RETURN
    GOSUB qr_length
    IF fn_ok = 0 THEN RETURN
    #x7 = #x3
    IF #x7 = 0 THEN RETURN
    qr_ok = 1
    WHILE #x7 > 0
        IF #x7 > 256 THEN
            #fz_w = 256
        ELSE
            #fz_w = #x7
        END IF
        #cp_dst = NB_BUF
        GOSUB qr_output
        IF fn_ok = 0 THEN
            qr_ok = 0
            #x7 = 0
        ELSE
            #x7 = #x7 - #fz_w
        END IF
    WEND
END

test_fuji_qrcode: PROCEDURE
    SECTION "qrcode low-level"
    #x4 = VARPTR str_qr_in(0) : #x5 = SL_qr_in
    GOSUB qr_input
    CHECK "qr input ok" @231, fn_ok
    qr_v = 0 : qr_e = 0 : qr_s = 0
    GOSUB qr_encode
    CHECK "qr encode ok" @238, fn_ok
    qr_m = 0
    GOSUB qr_length
    CHECK "qr length ok" @246, fn_ok
    #x0 = #x3                           ' enc_len
    CHECK "length matches" @247, #x0 = QL_bin
    #cp_dst = QR_OUT1 : #cp_n = 200 : GOSUB buf_zero
    #fz_w = #x0 : #cp_dst = QR_OUT1
    GOSUB qr_output
    CHECK "qr output ok" @255, fn_ok
    #sa = QR_OUT1 : #sb = VARPTR qr_bin(0) : #sn = QL_bin : GOSUB str_eq
    CHECK "binary matches" @257, t_same

    SECTION "qrcode_create"
    ' qrcode_create: input, encode, length, output into QR_OUT2
    #cp_dst = QR_OUT2 : #cp_n = 64 : GOSUB buf_zero
    #x4 = VARPTR str_qr_in(0) : #x5 = SL_qr_in
    qr_v = 0 : qr_e = 0 : qr_s = 0 : qr_m = 0
    GOSUB qr_ready
    #x1 = 0
    IF qr_ok THEN
        #fz_w = #x3 : #cp_dst = QR_OUT2
        GOSUB qr_output
        IF fn_ok THEN #x1 = #x3
    END IF
    CHECK "create ok" @269, #x1 > 0
    CHECK "create length" @271, #x1 = #x0
    #sa = QR_OUT1 : #sb = QR_OUT2 : #sn = #x1 : GOSUB str_eq
    CHECK "create output" @272, t_same

    SECTION "qrcode modes/ECC"
    qr_v = 0 : qr_e = 0 : qr_m = 2
    #x6 = VARPTR qr_bmp(0) : #x7 = QL_bmp : GOSUB qr_check_mode
    CHECK "bitmap matches" @278, qr_ok
    qr_m = 4
    #x6 = VARPTR qr_ata(0) : #x7 = QL_ata : GOSUB qr_check_mode
    CHECK "ATASCII matches" @281, qr_ok
    qr_m = 5
    #x6 = VARPTR qr_pet(0) : #x7 = QL_pet : GOSUB qr_check_mode
    CHECK "PETSCII matches" @284, qr_ok

    qr_m = 0
    qr_v = 0 : qr_e = 3
    #x6 = VARPTR qr_hig(0) : #x7 = QL_hig : GOSUB qr_check_mode
    CHECK "auto HIGH" @289, qr_ok
    qr_e = 1
    #x6 = VARPTR qr_qrt(0) : #x7 = QL_qrt : GOSUB qr_check_mode
    CHECK "auto MEDIUM" @293, qr_ok
    qr_e = 2
    GOSUB qr_check_mode
    CHECK "auto QUARTILE" @296, qr_ok

    qr_v = 2 : qr_e = 1
    #x6 = VARPTR qr_med(0) : #x7 = QL_med : GOSUB qr_check_mode
    CHECK "v2 MEDIUM" @300, qr_ok
    qr_e = 2
    #x6 = VARPTR qr_qrt(0) : #x7 = QL_qrt : GOSUB qr_check_mode
    CHECK "v2 QUARTILE" @303, qr_ok
    qr_v = 3 : qr_e = 3
    #x6 = VARPTR qr_hig(0) : #x7 = QL_hig : GOSUB qr_check_mode
    CHECK "v3 HIGH" @306, qr_ok
    qr_v = 2 : qr_e = 0
    #x6 = VARPTR qr_v2(0) : #x7 = QL_v2 : GOSUB qr_check_mode
    CHECK "explicit v2" @309, qr_ok

    SECTION "qr ANSI/SVG/short"
    #x4 = VARPTR str_qr_in(0) : #x5 = SL_qr_in
    qr_v = 0 : qr_e = 0 : qr_s = 0 : qr_m = 1
    GOSUB qr_drain
    CHECK "ANSI round-trip" @317, qr_ok
    qr_m = 3
    GOSUB qr_drain
    CHECK "SVG round-trip" @321, qr_ok
    #x4 = VARPTR str_qr_url(0) : #x5 = SL_qr_url
    qr_m = 0 : qr_s = 1
    GOSUB qr_drain
    CHECK "shorten trip" @325, qr_ok
END

test_qrcode: PROCEDURE
    GOSUB test_fuji_qrcode
END
