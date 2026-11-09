' wifi: tests/wifi.c
    DIM wf_saved, wf_have, wf_set, wf_stay, wf_back, wf_ssid, wf_pass, wf_rest, wf_rcon

test_fuji_wifi: PROCEDURE
    SECTION "fuji WiFi queries"
    mb_cmd = FUJICMD_GET_WIFI_ENABLED
    GOSUB fuji_byte0
    CHECK "wifi enabled" @43, #fz_w <> 0

    mb_cmd = FUJICMD_GET_WIFISTATUS
    GOSUB fuji_byte0
    CHECK "wifi status ok" @50, fn_ok
    CHECK "status is 3" @51, #fz_w = 3

    #cp_dst = NC_SAVED : #cp_n = 97 : GOSUB buf_zero
    #cp_dst = NC_SAVED
    GOSUB fuji_get_ssid
    CHECK "get_ssid ok" @58, fn_ok
    CHECK "ssid not empty" @59, (PEEK(NC_SAVED) AND 255) <> 0
END

test_fuji_network_scan: PROCEDURE
    SECTION "fuji WiFi scan"
    mb_cmd = FUJICMD_SCAN_NETWORKS
    GOSUB fuji_byte0
    CHECK "scan ok" @136, fn_ok
    CHECK "networks seen" @137, #fz_w > 0
    IF #fz_w > 0 THEN
        #cp_dst = SSID_INFO : #cp_n = 34 : GOSUB buf_zero
        wf_n = 0
        GOSUB fuji_get_scan_result
        CHECK "scan result ok" @146, fn_ok
        CHECK "scan ssid set" @147, (PEEK(SSID_INFO) AND 255) <> 0
    END IF
    GOSUB wait_for_wifi
    CHECK "wifi after scan" @151, wf_up
END

test_fuji_set_ssid: PROCEDURE
    SECTION "fuji_set_ssid"
    #cp_dst = NC_SAVED : #cp_n = 97 : GOSUB buf_zero
    #cp_dst = NC_SAVED
    GOSUB fuji_get_ssid
    wf_saved = fn_ok
    wf_have = 0
    IF wf_saved THEN
        IF (PEEK(NC_SAVED) AND 255) <> 0 THEN wf_have = 1
    END IF
    wf_set = 0 : wf_stay = 0 : wf_back = 0 : wf_ssid = 0 : wf_pass = 0
    wf_rest = 0 : wf_rcon = 0

    IF wf_have THEN
        #fn_src = NC_SAVED
        GOSUB fuji_set_ssid
        wf_set = fn_ok
        GOSUB wait_for_wifi
        wf_stay = wf_up

        #cp_dst = NC_BACK : #cp_n = 97 : GOSUB buf_zero
        #cp_dst = NC_BACK
        GOSUB fuji_get_ssid
        wf_back = fn_ok
        IF wf_back THEN
            #sa = NC_SAVED : #sb = NC_BACK : #sn = 33
            GOSUB str_eq
            wf_ssid = t_same
            #sa = NC_SAVED + 33 : #sb = NC_BACK + 33 : #sn = 64
            GOSUB str_eq
            wf_pass = t_same
        END IF

        ' Reapply the saved credentials before any check can abort.
        #fn_src = NC_SAVED
        GOSUB fuji_set_ssid
        wf_rest = fn_ok
        GOSUB wait_for_wifi
        wf_rcon = wf_up
    END IF

    CHECK "read saved cred" @110, wf_saved
    CHECK "ssid saved" @111, wf_have
    CHECK "set_ssid ok" @112, wf_set
    CHECK "reconnects" @113, wf_stay
    CHECK "read after set" @114, wf_back
    CHECK "ssid matches" @115, wf_ssid
    CHECK "pass matches" @116, wf_pass
    CHECK "restore ok" @117, wf_rest
    CHECK "connected again" @118, wf_rcon
END

test_wifi: PROCEDURE
    GOSUB test_fuji_wifi : IF t_abort THEN RETURN
    GOSUB test_fuji_network_scan : IF t_abort THEN RETURN
    GOSUB test_fuji_set_ssid
END
