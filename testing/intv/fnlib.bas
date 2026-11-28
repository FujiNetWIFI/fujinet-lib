' fnlib.bas -- FujiNet mailbox transport and fujinet-lib-shaped calls.
'
' The mailbox is the pico cart's fuji_mailbox.h. MEMATTR stops at $9BFF:
' RAM over $9C00-$9FFF would shadow the emulator's FujiNet peripheral.
    ASM MEMATTR $8000, $9BFF, "+RWN"

    CONST FN_MAGIC0     = $9C00
    CONST FN_MAGIC1     = $9C01
    CONST FN_SEQ        = $9C03
    CONST FN_ACKSEQ     = $9C04
    CONST FN_DEVICE     = $9C05
    CONST FN_CMD        = $9C06
    CONST FN_NPARAM     = $9C07
    CONST FN_TXLEN_LO   = $9C08
    CONST FN_TXLEN_HI   = $9C09
    CONST FN_ERR        = $9C0B
    CONST FN_RXLEN_LO   = $9C0C
    CONST FN_RXLEN_HI   = $9C0D
    CONST FN_REPLY_CMD  = $9C0E
    CONST FN_PARAM_SIZE = $9C10
    CONST FN_PARAM_VAL  = $9C20
    CONST FN_TX         = $9C40
    CONST FN_RX         = $9D40

    CONST FUJICMD_ACK = $06

    CONST FUJI_DEVICEID        = $70
    CONST FUJICMD_OPEN_APPKEY  = $DC
    CONST FUJICMD_WRITE_APPKEY = $DE
    CONST FUJICMD_READ_APPKEY  = $DD

    ' Cart RAM buffers ($9000-$9AFF is ours, $9B00 is the result block).
    CONST AK_WRITE = $9300
    CONST AK_READ  = $9340

    ' fn_ok: last transaction was ACKed. fn_nak: the device answered NAK.
    ' fn_to: no answer in time. #mb_err: FN_ERR of a NAK.
    DIM fn_ok, fn_nak, fn_to
    DIM #mb_err
    DIM mb_dev, mb_cmd, mb_nparam, mb_seq
    DIM #fn_txlen
    DIM #fn_t
    DIM #fn_src
    DIM #fn_i
    DIM pm_i, pm_size
    DIM #pm_val
    DIM #cs_len, #fz_w

' Wait up to 3 s for the 'F','N' magic bytes. Sets fn_ok.
fn_wait_mailbox: PROCEDURE
    fn_ok = 0
    #fn_t = 0
    WHILE (fn_ok = 0) AND (#fn_t < 180)
        IF (PEEK(FN_MAGIC0) AND 255) = 70 THEN
            IF (PEEK(FN_MAGIC1) AND 255) = 78 THEN fn_ok = 1
        END IF
        IF fn_ok = 0 THEN
            #fn_t = #fn_t + 1
            WAIT
        END IF
    WEND
END

' Issue the transaction in mb_dev/mb_cmd/mb_nparam/#fn_txlen (payload staged
' at FN_TX) and wait for the reply. SEQ comes from the cart's ACKSEQ, since
' a console reset zeroes our variables but not the cart.
fn_transact: PROCEDURE
    POKE FN_DEVICE, mb_dev
    POKE FN_CMD, mb_cmd
    POKE FN_NPARAM, mb_nparam
    POKE FN_TXLEN_LO, #fn_txlen AND 255
    POKE FN_TXLEN_HI, #fn_txlen / 256

    mb_seq = (PEEK(FN_ACKSEQ) AND 255) + 1
    IF mb_seq = 0 THEN mb_seq = 1
    POKE FN_SEQ, mb_seq

    fn_ok = 0 : fn_nak = 0 : fn_to = 0 : #mb_err = 0
    #fn_t = 0
    WHILE ((PEEK(FN_ACKSEQ) AND 255) <> mb_seq) AND (#fn_t < 900)
        #fn_t = #fn_t + 1
        WAIT
    WEND

    IF #fn_t >= 900 THEN
        fn_to = 1
        RETURN
    END IF
    IF (PEEK(FN_REPLY_CMD) AND 255) <> FUJICMD_ACK THEN
        fn_nak = 1
        #mb_err = PEEK(FN_ERR) AND 255
        RETURN
    END IF
    fn_ok = 1
END

' Stage parameter pm_i (pm_size 1 or 2 bytes, little-endian) = #pm_val.
fn_param: PROCEDURE
    POKE FN_PARAM_SIZE + pm_i, pm_size
    POKE FN_PARAM_VAL + pm_i * 4, #pm_val AND 255
    IF pm_size > 1 THEN POKE FN_PARAM_VAL + pm_i * 4 + 1, #pm_val / 256
END

' ---- AppKey (fujinet-lib common/fuji_appkey.c) ----
' Creator/app and key size are set once, as fuji_set_appkey_details does.
    DIM #ak_creator, ak_app
    DIM ak_key, ak_mode

' Open the key in ak_mode (0 read, 1 write). The struct is exactly 6 bytes.
ak_open: PROCEDURE
    mb_dev = FUJI_DEVICEID
    mb_cmd = FUJICMD_OPEN_APPKEY
    mb_nparam = 0
    POKE FN_TX + 0, #ak_creator AND 255
    POKE FN_TX + 1, #ak_creator / 256
    POKE FN_TX + 2, ak_app
    POKE FN_TX + 3, ak_key
    POKE FN_TX + 4, ak_mode
    POKE FN_TX + 5, 0
    #fn_txlen = 6
    GOSUB fn_transact
END

' fuji_write_appkey(ak_key, #cs_len, #fn_src) -> fn_ok
fuji_write_appkey: PROCEDURE
    ak_mode = 1
    GOSUB ak_open
    IF fn_ok = 0 THEN RETURN
    mb_cmd = FUJICMD_WRITE_APPKEY
    mb_nparam = 0
    IF #cs_len > 0 THEN
        FOR #fn_i = 1 TO #cs_len
            POKE FN_TX + #fn_i - 1, PEEK(#fn_src + #fn_i - 1) AND 255
        NEXT #fn_i
    END IF
    #fn_txlen = #cs_len
    GOSUB fn_transact
END

' fuji_read_appkey(ak_key) -> fn_ok, #cs_len, data in AK_READ. The RS232
' bridge prepends a 2-byte length to the key bytes.
fuji_read_appkey: PROCEDURE
    ak_mode = 0
    GOSUB ak_open
    IF fn_ok = 0 THEN RETURN
    mb_cmd = FUJICMD_READ_APPKEY
    mb_nparam = 0
    #fn_txlen = 0
    GOSUB fn_transact
    IF fn_ok = 0 THEN RETURN
    #cs_len = (PEEK(FN_RX) AND 255) + (PEEK(FN_RX + 1) AND 255) * 256
    IF #cs_len > 64 THEN
        #fn_t = 64
    ELSE
        #fn_t = #cs_len
    END IF
    IF #fn_t > 0 THEN
        FOR #fn_i = 1 TO #fn_t
            POKE AK_READ + #fn_i - 1, PEEK(FN_RX + 1 + #fn_i) AND 255
        NEXT #fn_i
    END IF
END

' ---- shared helpers ----
    DIM #cp_dst, #cp_n

' Copy min(#cp_n, RXLEN) bytes of the last reply to #cp_dst, then a NUL at
' #cp_dst + #cp_n, as the lib terminates the time strings it returns.
rx_to_buf: PROCEDURE
    GOSUB rx_copy
    POKE #cp_dst + #cp_n, 0
END

' Copy min(#cp_n, RXLEN) bytes of the last reply to #cp_dst; #fn_t = count.
rx_copy: PROCEDURE
    #fn_t = (PEEK(FN_RXLEN_LO) AND 255) + (PEEK(FN_RXLEN_HI) AND 255) * 256
    IF #fn_t > #cp_n THEN #fn_t = #cp_n
    IF #fn_t > 0 THEN
        FOR #fn_i = 1 TO #fn_t
            POKE #cp_dst + #fn_i - 1, PEEK(FN_RX + #fn_i - 1) AND 255
        NEXT #fn_i
    END IF
END

' Zero #cp_n bytes at #cp_dst.
buf_zero: PROCEDURE
    FOR #fn_i = 1 TO #cp_n
        POKE #cp_dst + #fn_i - 1, 0
    NEXT #fn_i
END

' Stage #fn_src[0..#cp_n-1] (ROM or RAM) as the transaction payload.
tx_put: PROCEDURE
    #fn_txlen = #cp_n
    IF #cp_n > 0 THEN
        FOR #fn_i = 1 TO #cp_n
            POKE FN_TX + #fn_i - 1, PEEK(#fn_src + #fn_i - 1) AND 255
        NEXT #fn_i
    END IF
END

' #cp_n = length of the NUL-terminated string at #fn_src (at most 255).
str_len: PROCEDURE
    #cp_n = 0
    WHILE (#cp_n < 255) AND ((PEEK(#fn_src + #cp_n) AND 255) <> 0)
        #cp_n = #cp_n + 1
    WEND
END

' ---- Clock (device $45; fujinet-lib common/fuji_clock.c) ----
    CONST CLOCK_DEVICEID = $45
    CONST CLK_BUF = $9500       ' 32 bytes: time_data
    CONST CLK_TZ0 = $9520       ' 64 bytes: original TZ
    CONST CLK_TZ1 = $9560       ' 64 bytes: read-back TZ
    CONST CLK_UTC = $9500+$80   ' 32: per-call TZ results
    CONST CLK_NY  = $95A0
    CONST CLK_TOK = $95C0
    CONST CLK_SYS = $95E0       ' 64 bytes
    CONST CLKCMD_SETTZ     = $99
    CONST CLKCMD_SETTZ_MAIN = $74
    CONST CLKCMD_GETTZ_LEN = $4C
    CONST CLKCMD_GETTZ     = $47

    DIM cf_fmt, cf_alt

' Time formats, in fujinet-lib's TimeFormat order: SIMPLE_BINARY,
' PRODOS_BINARY, APETIME_BINARY, TZ_ISO_STRING, UTC_ISO_STRING,
' APPLE3_SOS_BINARY, SIMPLE_BINARY_WITH_HUNDREDTHS.
clk_cmd_tab:
    DATA $54, $50, $93, $49, $5A, $53, $4D
clk_len_tab:
    DATA 7, 4, 6, 25, 25, 19, 8

' clock_get_time(CLK_BUF, cf_fmt, cf_alt) -> fn_ok
clock_get_time: PROCEDURE
    #cp_dst = CLK_BUF : #cp_n = 32 : GOSUB buf_zero
    mb_dev = CLOCK_DEVICEID
    mb_cmd = PEEK(VARPTR clk_cmd_tab(0) + cf_fmt) AND 255
    mb_nparam = 1
    pm_i = 0 : pm_size = 1 : #pm_val = cf_alt : GOSUB fn_param
    #fn_txlen = 0
    GOSUB fn_transact
    IF fn_ok = 0 THEN RETURN
    #cp_dst = CLK_BUF
    #cp_n = PEEK(VARPTR clk_len_tab(0) + cf_fmt) AND 255
    GOSUB rx_to_buf
END

' Set a TZ from the #cs_len bytes at #fn_src with mb_cmd (CLKCMD_SETTZ for
' the per-call zone, CLKCMD_SETTZ_MAIN for the system one) -> fn_ok
clock_set_tz_cmd: PROCEDURE
    mb_dev = CLOCK_DEVICEID
    mb_nparam = 1
    pm_i = 0 : pm_size = 2 : #pm_val = #cs_len : GOSUB fn_param
    #cp_n = #cs_len
    GOSUB tx_put
    GOSUB fn_transact
END

' clock_set_tz(#fn_src, #cs_len) -> fn_ok
clock_set_tz: PROCEDURE
    mb_cmd = CLKCMD_SETTZ_MAIN
    GOSUB clock_set_tz_cmd
END

' clock_get_tz(#cp_dst: 64 bytes) -> fn_ok
clock_get_tz: PROCEDURE
    mb_dev = CLOCK_DEVICEID
    mb_cmd = CLKCMD_GETTZ_LEN
    mb_nparam = 0
    #fn_txlen = 0
    GOSUB fn_transact
    IF fn_ok = 0 THEN RETURN
    #cs_len = PEEK(FN_RX) AND 255
    IF #cs_len > 63 THEN #cs_len = 63
    mb_cmd = CLKCMD_GETTZ
    GOSUB fn_transact
    IF fn_ok = 0 THEN RETURN
    #cp_n = #cs_len
    GOSUB rx_to_buf
END

' clock_get_time_tz(CLK_BUF, tz at #fn_src/#cs_len, cf_fmt) -> fn_ok
clock_get_time_tz: PROCEDURE
    mb_cmd = CLKCMD_SETTZ
    GOSUB clock_set_tz_cmd
    IF fn_ok = 0 THEN RETURN
    cf_alt = 1
    GOSUB clock_get_time
END

' fuji_sleep_ms for whole seconds: 60 frames a second.
sleep_secs: PROCEDURE
    FOR #fn_i = 1 TO #cp_n * 60
        WAIT
    NEXT #fn_i
END

' ---- Fuji device queries ----
    CONST FUJICMD_GET_ADAPTERCONFIG = $E8

' fuji_get_adapter_config -> fn_ok, struct at FN_RX
fuji_get_adapter_config: PROCEDURE
    mb_dev = FUJI_DEVICEID
    mb_cmd = FUJICMD_GET_ADAPTERCONFIG
    mb_nparam = 0
    #fn_txlen = 0
    GOSUB fn_transact
END

' ---- WiFi ----
    CONST FUJICMD_GET_SSID        = $FE
    CONST FUJICMD_SCAN_NETWORKS   = $FD
    CONST FUJICMD_GET_SCAN_RESULT = $FC
    CONST FUJICMD_SET_SSID        = $FB
    CONST FUJICMD_GET_WIFISTATUS  = $FA
    CONST FUJICMD_GET_WIFI_ENABLED = $EA

    CONST NC_SAVED = $9600      ' 97 bytes: NetConfig as stored
    CONST NC_BACK  = $9670      ' 97 bytes: NetConfig read back
    CONST SSID_INFO = $96E0     ' 34 bytes: SSIDInfo


' Fuji device call with no payload; mb_cmd set by the caller -> fn_ok
fuji_cmd0: PROCEDURE
    mb_dev = FUJI_DEVICEID
    mb_nparam = 0
    #fn_txlen = 0
    GOSUB fn_transact
END

' Reply byte 0 -> #fz_w (WiFi enabled / status / scan count)
fuji_byte0: PROCEDURE
    GOSUB fuji_cmd0
    #fz_w = 0
    IF fn_ok THEN #fz_w = PEEK(FN_RX) AND 255
END

' fuji_get_ssid -> fn_ok, NetConfig (97 bytes) to #cp_dst
fuji_get_ssid: PROCEDURE
    mb_cmd = FUJICMD_GET_SSID
    GOSUB fuji_cmd0
    IF fn_ok THEN
        #cp_n = 97
        GOSUB rx_copy
    END IF
END

' fuji_get_scan_result(wf_n) -> fn_ok, SSIDInfo (34 bytes) at SSID_INFO
    DIM wf_n
fuji_get_scan_result: PROCEDURE
    mb_dev = FUJI_DEVICEID
    mb_cmd = FUJICMD_GET_SCAN_RESULT
    mb_nparam = 1
    pm_i = 0 : pm_size = 1 : #pm_val = wf_n : GOSUB fn_param
    #fn_txlen = 0
    GOSUB fn_transact
    IF fn_ok THEN
        #cp_dst = SSID_INFO : #cp_n = 34
        GOSUB rx_copy
    END IF
END

' fuji_set_ssid: the 97-byte NetConfig at #fn_src. The firmware wants at
' least one parameter, though it ignores the value.
fuji_set_ssid: PROCEDURE
    mb_dev = FUJI_DEVICEID
    mb_cmd = FUJICMD_SET_SSID
    mb_nparam = 1
    pm_i = 0 : pm_size = 1 : #pm_val = 0 : GOSUB fn_param
    #cp_n = 97
    GOSUB tx_put
    GOSUB fn_transact
END

' wait_for_wifi (tests/wifi.c): wf_up = connected within 20 tries, 2.5 s apart
    DIM wf_up, wf_try
wait_for_wifi: PROCEDURE
    wf_up = 0
    FOR wf_try = 1 TO 20
        mb_cmd = FUJICMD_GET_WIFISTATUS
        GOSUB fuji_byte0
        IF fn_ok THEN
            IF #fz_w = 3 THEN
                wf_up = 1
                RETURN
            END IF
        END IF
        FOR #fn_i = 1 TO 150
            WAIT
        NEXT #fn_i
    NEXT wf_try
END

' ---- Fuji device primitives (device $70) ----
' Set mb_cmd, fz_n (0-3 one-byte parameters fz_a..fz_c), then GOSUB fz_set,
' stage the payload, and GOSUB fz_go -> fn_ok. A reply is copied with
' rx_copy (#cp_dst / #cp_n).
    DIM fz_a, fz_b, fz_c, fz_n

fz_set: PROCEDURE
    mb_nparam = fz_n
    #fn_txlen = 0
    IF fz_n > 0 THEN
        pm_i = 0 : pm_size = 1 : #pm_val = fz_a : GOSUB fn_param
    END IF
    IF fz_n > 1 THEN
        pm_i = 1 : pm_size = 1 : #pm_val = fz_b : GOSUB fn_param
    END IF
    IF fz_n > 2 THEN
        pm_i = 2 : pm_size = 1 : #pm_val = fz_c : GOSUB fn_param
    END IF
END

' One two-byte parameter, #fz_w (the lib's B12)
fz_b12: PROCEDURE
    mb_nparam = 1
    #fn_txlen = 0
    pm_i = 0 : pm_size = 2 : #pm_val = #fz_w : GOSUB fn_param
END

fz_go: PROCEDURE
    mb_dev = FUJI_DEVICEID
    GOSUB fn_transact
END

' Payload: the #cs_len bytes at #fn_src, NUL-padded to 256 (MAX_FILENAME_LEN)
fz_pad: PROCEDURE
    #cp_n = #cs_len
    GOSUB tx_put
    FOR #fn_i = #cs_len TO 255
        POKE FN_TX + #fn_i, 0
    NEXT #fn_i
    #fn_txlen = 256
END

' Fuji call with no parameters and no payload -> fn_ok
fd_plain: PROCEDURE
    fz_n = 0
    GOSUB fz_set
    GOSUB fz_go
END

' Fuji call with the one-byte parameter fz_a -> fn_ok
fd_a1: PROCEDURE
    fz_n = 1
    GOSUB fz_set
    GOSUB fz_go
END

