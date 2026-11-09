' html program: tests/http.c and tests/html.c
    DIM hs_ok, hq_i, hq_m, hq_go
    DIM hc_am, hc_as, hc_ac, hc_bm, hc_bs, hc_bc, hc_good, hc_dm
    DIM hc_sm, hc_ss, hc_sc, hc_lm, hc_ls, hc_lc
    DIM #hc_el, #hc_sub, #hc_cs, #hc_step

STR sgml_url_h "N1:https://httpbin.dev/get?a=I%20%3C3%20FujiNet&b=a%20%3E%20b%20%3Ci%3Ec%3C%2Fi%3E%20d&c=%3Cp%3EHello%20%3Cb%3Eworld%3C%2Fb%3E%3C%2Fp%3E&d=I%20%3C3%20%3Cb%3EFujiNet%3C%2Fb%3E"
STR post_json_h "{\"test\":\"fujinet_integration\"}"
STR hdr_x_h "X-FujiNet-Test: integration"
STR hdr_accept_h "Accept: text/html"
STR hdr_ctype_h "Content-Type: application/json"
STR put_body_h "{\"test\":\"fujinet_put\"}"
STR put_plain_h "fujinet=put_test"
STR put_forbid_h "fujinet=forbidden"
STR q_title_h "/slideshow/title"
STR q_nokey_h "/slideshow/no_such_key"
STR q_clen_h "/headers/Content-Length/0"
STR q_jtest_h "/json/test"
STR q_ctype_h "/headers/Content-Type/0"
STR a_b_h "/args/b"
STR a_c_h "/args/c"
STR a_a_h "/args/a"
STR a_d_h "/args/d"
STR w_b_h "a > b c d"
STR w_c_h "Hello world"
STR w_a_h "I <3 FujiNet"
STR v_1200_h "1200"
STR v_fjput_h "fujinet_put"
STR v_appjson_h "application/json"
STR abc_h "ABC"
STR q_h1_h "h1"
STR q_a_h "a"
STR q_nth_h "a:nth-of-type(2)"
STR moby_h "Moby-Dick"
STR bin4_h "\x01\x02\x03\xFF"

' Open the spec at #fn_src/#cs_len with nw_mode and nw_trans
ht_open: PROCEDURE
    nw_trans = 0
    GOSUB net_open
END

' Add one line header at #fn_src/#cs_len (a plain write)
ht_header: PROCEDURE
    #cp_n = #cs_len
    GOSUB net_write
END

' Read up to #cp_n bytes at NB_BUF (zeroed first) -> #rd_n
ht_read: PROCEDURE
    #cp_dst = NB_BUF : #cp_n = 512 : GOSUB buf_zero
    #cp_dst = NB_BUF : #cp_n = 512 : GOSUB net_read
END

' the spec at #fn_src is the unit-1 spec of the open channel
test_network_http_get: PROCEDURE
    SECTION "HTTP GET read"
    nw_mode = 4 : nw_trans = 2
    #fn_src = VARPTR str_devspec(0) : #cs_len = SL_devspec
    GOSUB net_open
    CHECK "open GET" @33, fn_ok
    GOSUB net_status
    CHECK "status after" @41, fn_ok
    CHECK "no error" @43, nw_err = 1
    GOSUB ht_read
    CHECK "read data" @51, #rd_n > 0
    GOSUB net_close
    CHECK "close GET" @65, fn_ok
END

' Held-peer GET: read 4 into NB_BUF, then STATUS must say EOF, 0 bytes,
' closed. Spec at #fn_src/#cs_len.
ht_held: PROCEDURE
    nw_mode = 4 : nw_trans = 0
    GOSUB net_open
    CHECKL "held GET opens|chunk GET opens" @86, hq_i, fn_ok
    #cp_dst = NB_BUF : #cp_n = 4 : GOSUB net_read
    #x1 = #rd_n
    GOSUB net_status
    #x2 = fn_ok
    #x3 = #nw_avail
    #x4 = nw_conn
    #x5 = nw_err
    GOSUB net_close
    CHECKL "fixed 3 bytes|chunk 3 bytes" @91, hq_i, #x1 = 3
    #sa = NB_BUF : #sb = VARPTR str_abc_h(0) : #sn = 3 : GOSUB str_eq
    CHECKL "fixed is ABC|chunk is ABC" @92, hq_i, t_same
    CHECKL "fixed status ok|chunk status ok" @93, hq_i, #x2
    CHECKL "fixed none left|chunk none left" @94, hq_i, #x3 = 0
    CHECKL "fixed done|chunk done" @95, hq_i, #x4 = 0
    CHECKL "fixed EOF|chunk EOF" @96, hq_i, #x5 = 136
    CHECKL "fixed closes|chunk closes" @97, hq_i, fn_ok
END

test_network_http_fixed_completion: PROCEDURE
    SECTION "HTTP fixed held"
    #fn_src = VARPTR str_fixed_held(0) : #cs_len = SL_fixed_held
    hq_i = 1
    GOSUB ht_held
END

test_network_http_chunked_completion: PROCEDURE
    SECTION "HTTP chunked held"
    #fn_src = VARPTR str_chunk_held(0) : #cs_len = SL_chunk_held
    hq_i = 2
    GOSUB ht_held
END

test_network_http_304: PROCEDURE
    SECTION "HTTP 304 no body"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_h304_url(0) : #cs_len = SL_h304_url
    GOSUB net_open
    hs_ok = fn_ok
    #x1 = 0                             ' status ok of the last poll
    #x2 = $FFFF                         ' avail
    #x3 = $FF                           ' conn
    IF hs_ok THEN
        hq_i = 0
        wd_go = 1
        WHILE wd_go
            GOSUB net_status
            #x1 = fn_ok
            #x2 = #nw_avail
            #x3 = nw_conn
            hq_i = hq_i + 1
            IF fn_ok = 0 THEN wd_go = 0
            IF #nw_avail <> 0 THEN wd_go = 0
            IF nw_conn <> 1 THEN wd_go = 0
            IF hq_i >= 100 THEN wd_go = 0
        WEND
    END IF
    GOSUB net_close
    CHECK "304 opens" @158, hs_ok
    CHECK "304 status ok" @159, #x1
    CHECK "304 avail 0" @160, #x2 = 0
    CHECK "304 completed" @161, #x3 = 0
END

' Read the clock (HUNDREDTHS): minutes, seconds, hundredths into hc_bm/bs/bc;
' hc_good = the call worked and every field is in range
ht_clock: PROCEDURE
    cf_fmt = 6 : cf_alt = 0
    GOSUB clock_get_time
    hc_good = fn_ok
    IF fn_ok = 0 THEN RETURN
    #fn_t = PEEK(CLK_BUF + 5) AND 255
    hc_bm = #fn_t
    #fn_t = PEEK(CLK_BUF + 6) AND 255
    hc_bs = #fn_t
    #fn_t = PEEK(CLK_BUF + 7) AND 255
    hc_bc = #fn_t
    IF hc_bm > 59 THEN hc_good = 0
    IF hc_bs > 59 THEN hc_good = 0
    IF hc_bc > 99 THEN hc_good = 0
    IF (PEEK(CLK_BUF + 4) AND 255) > 23 THEN hc_good = 0
END

' hc_elapsed: centiseconds from (hc_am, hc_as, hc_ac) to (hc_bm, hc_bs, hc_bc)
' -> #hc_el; hc_good = it is later and under 5 minutes. (The C test also
' range-checks the calendar date; only minutes and below are kept here.)
hc_elapsed: PROCEDURE
    hc_good = 0
    hc_dm = hc_bm + 60 - hc_am
    IF hc_dm >= 60 THEN hc_dm = hc_dm - 60
    IF hc_dm > 4 THEN RETURN
    #hc_el = (hc_dm * 60 + hc_bs) * 100 + hc_bc
    #hc_sub = hc_as * 100 + hc_ac
    IF #hc_el <= #hc_sub THEN RETURN
    #hc_el = #hc_el - #hc_sub
    hc_good = 1
END

test_network_http_304_completion: PROCEDURE
    SECTION "HTTP304 under 5 s"
    #x0 = 0                             ' result: 1 completed, 2 too slow
    #hc_cs = 0
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_h304_held(0) : #cs_len = SL_h304_held
    GOSUB net_open
    IF fn_ok THEN
        GOSUB ht_clock
        IF hc_good THEN
            hc_sm = hc_bm : hc_ss = hc_bs : hc_sc = hc_bc
            hc_lm = hc_bm : hc_ls = hc_bs : hc_lc = hc_bc
            hq_i = 0
            wd_go = 1
            WHILE wd_go
                GOSUB net_status
                #x6 = fn_ok
                GOSUB ht_clock
                IF hc_good = 0 THEN
                    wd_go = 0
                ELSE
                    hc_am = hc_lm : hc_as = hc_ls : hc_ac = hc_lc
                    GOSUB hc_elapsed
                    IF hc_good = 0 THEN
                        wd_go = 0
                    ELSE
                        hc_am = hc_sm : hc_as = hc_ss : hc_ac = hc_sc
                        GOSUB hc_elapsed
                        #hc_cs = #hc_el
                        IF hc_good = 0 THEN
                            wd_go = 0
                        ELSEIF #x6 = 0 THEN
                            wd_go = 0
                        ELSEIF nw_conn > 1 THEN
                            wd_go = 0
                        ELSEIF #hc_cs >= 500 THEN
                            #x0 = 2
                            wd_go = 0
                        ELSEIF #hc_cs >= 499 THEN
                            wd_go = 0
                        ELSEIF nw_conn = 0 THEN
                            #x0 = 1
                            wd_go = 0
                        ELSE
                            hc_lm = hc_bm : hc_ls = hc_bs : hc_lc = hc_bc
                        END IF
                    END IF
                END IF
                hq_i = hq_i + 1
                IF hq_i >= 100 THEN wd_go = 0
            WEND
        END IF
    END IF
    GOSUB net_close
    CHECK "304 timing ok" @270, #x0 <> 0
    CHECK "304 under 5 s" @271, #x0 = 1
END

test_network_http_get_nonblocking: PROCEDURE
    SECTION "HTTP GET nonblock"
    nw_mode = 4 : nw_trans = 2
    #fn_src = VARPTR str_devspec(0) : #cs_len = SL_devspec
    GOSUB net_open
    CHECK "open GET nb" @290, fn_ok
    GOSUB net_status
    CHECK "status nb ok" @298, fn_ok
    CHECK "no error nb" @300, nw_err = 1
    IF #nw_avail > 0 THEN
        #cp_n = #nw_avail
        IF #cp_n > 512 THEN #cp_n = 512
        #cp_dst = NB_BUF
        GOSUB net_read_nb
        CHECK "read_nb >= 0" @311, rd_bad = 0
    END IF
    GOSUB net_close
    CHECK "close nb GET" @322, fn_ok
END

test_network_http_post: PROCEDURE
    SECTION "HTTP POST"
    nw_mode = $0D : nw_trans = 0
    #fn_src = VARPTR str_post_url(0) : #cs_len = SL_post_url
    GOSUB net_open
    CHECK "open POST" @340, fn_ok
    #fn_src = VARPTR str_post_json_h(0) : #cp_n = SL_post_json_h
    GOSUB net_http_post
    CHECK "http_post ok" @347, fn_ok
    GOSUB ht_read
    CHECK "read after POST" @355, #rd_n > 0
    GOSUB net_close
    CHECK "close POST" @362, fn_ok

    nw_mode = $0D : nw_trans = 0
    #fn_src = VARPTR str_post_url(0) : #cs_len = SL_post_url
    GOSUB net_open
    CHECK "open bin POST" @366, fn_ok
    #fn_src = VARPTR str_bin4_h(0) : #cp_n = 4
    GOSUB net_http_post
    CHECK "post_bin ok" @373, fn_ok
    GOSUB ht_read
    CHECK "read bin POST" @377, #rd_n > 0
    GOSUB net_close
    CHECK "close bin POST" @380, fn_ok
END

test_network_http_post_large: PROCEDURE
    SECTION "HTTP POST 1200"
    nw_mode = $0D : nw_trans = 0
    #fn_src = VARPTR str_post_url(0) : #cs_len = SL_post_url
    GOSUB net_open
    CHECK "open POST" @405, fn_ok

    ' 1200 'Q' bytes: set-data mode, five 240-byte writes, body mode
    nw_a = 4 : GOSUB net_http_mode
    #x6 = fn_ok
    FOR #fn_i = 0 TO 239
        POKE NB_BUF + #fn_i, 81
    NEXT #fn_i
    FOR hq_i = 1 TO 5
        #fn_src = NB_BUF : #cp_n = 240 : GOSUB net_write
        IF fn_ok = 0 THEN #x6 = 0
    NEXT hq_i
    nw_a = 0 : GOSUB net_http_mode
    IF fn_ok = 0 THEN #x6 = 0
    CHECK "POST 1200 ok" @408, #x6

    GOSUB net_json_parse
    CHECK "json parse echo" @413, fn_ok
    #fn_src = VARPTR str_q_clen_h(0) : #cs_len = SL_q_clen_h
    GOSUB net_json_query
    #x0 = 0
    IF jq_bad = 0 THEN
        IF #jq_n > 0 THEN
            #sa = NB_BUF : #sb = VARPTR str_v_1200_h(0) : #sn = 5 : GOSUB str_eq
            #x0 = t_same
        END IF
    END IF
    CHECK "server got 1200" @417, #x0
    GOSUB net_status
    CHECK "still answers" @421, fn_ok
    GOSUB net_close
END

test_network_http_headers: PROCEDURE
    SECTION "HTTP headers"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_devspec(0) : #cs_len = SL_devspec
    GOSUB net_open
    CHECK "open for hdrs" @440, fn_ok
    nw_a = 3 : GOSUB net_http_mode
    CHECK "start headers" @447, fn_ok
    #fn_src = VARPTR str_hdr_x_h(0) : #cs_len = SL_hdr_x_h : GOSUB ht_header
    CHECK "add header" @454, fn_ok
    #fn_src = VARPTR str_hdr_accept_h(0) : #cs_len = SL_hdr_accept_h : GOSUB ht_header
    CHECK "add header 2" @457, fn_ok
    nw_a = 0 : GOSUB net_http_mode
    CHECK "end headers" @464, fn_ok
    GOSUB ht_read
    CHECK "read w/ hdrs" @472, #rd_n > 0
    GOSUB net_close
    CHECK "close hdrs" @479, fn_ok
END

test_network_http_channel_mode: PROCEDURE
    SECTION "HTTP channel mode"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_devspec(0) : #cs_len = SL_devspec
    GOSUB net_open
    CHECK "open chan mode" @495, fn_ok
    nw_a = 1 : GOSUB net_http_mode
    CHECK "mode headers" @503, fn_ok
    nw_a = 0 : GOSUB net_http_mode
    CHECK "mode body" @506, fn_ok
    GOSUB net_close
    CHECK "close chan mode" @513, fn_ok
END

test_network_json: PROCEDURE
    SECTION "JSON parse/query"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_json_url(0) : #cs_len = SL_json_url
    GOSUB net_open
    CHECK "open JSON" @533, fn_ok
    GOSUB net_json_parse
    CHECK "json parse" @540, fn_ok
    nw_unit = 1
    GOSUB net_status
    CHECK "status after" @548, fn_ok
    #x0 = 0
    IF nw_err = 1 THEN #x0 = 1
    IF nw_err = 136 THEN #x0 = 1
    CHECK "no error json" @550, #x0

    #fn_src = VARPTR str_q_title_h(0) : #cs_len = SL_q_title_h
    GOSUB net_json_query
    CHECK "query data" @558, #jq_n > 0
    CHECK "query non-empty" @559, (PEEK(NB_BUF) AND 255) <> 0

    FOR #fn_i = 0 TO 15
        POKE NB_BUF + #fn_i, 88
    NEXT #fn_i
    #fn_src = VARPTR str_q_nokey_h(0) : #cs_len = SL_q_nokey_h
    GOSUB net_json_query
    CHECK "nomatch is 0" @566, #jq_n = 0
    CHECK "nomatch empty" @567, (PEEK(NB_BUF) AND 255) = 0
    GOSUB net_close
    CHECK "close JSON" @574, fn_ok
END

' json_stripped: query #x4/#x5, want #x6/#x7 -> t_in
ht_stripped: PROCEDURE
    #fn_src = #x4 : #cs_len = #x5
    GOSUB net_json_query
    t_in = 0
    IF jq_bad = 0 THEN
        IF #jq_n = #x7 THEN
            #sa = NB_BUF : #sb = #x6 : #sn = #x7 + 1 : GOSUB str_eq
            t_in = t_same
        END IF
    END IF
END

test_network_json_strip_tags: PROCEDURE
    SECTION "JSON strip SGML tags"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_sgml_url_h(0) : #cs_len = SL_sgml_url_h
    GOSUB net_open
    CHECK "strip open" @609, fn_ok
    GOSUB net_json_parse
    CHECK "strip parse" @611, fn_ok
    nw_a = 4 : GOSUB net_set_query_param
    CHECK "strip flag ok" @613, fn_ok
    #x4 = VARPTR str_a_b_h(0) : #x5 = SL_a_b_h : #x6 = VARPTR str_w_b_h(0) : #x7 = SL_w_b_h
    GOSUB ht_stripped
    CHECK "stray > kept" @616, t_in
    #x4 = VARPTR str_a_c_h(0) : #x5 = SL_a_c_h : #x6 = VARPTR str_w_c_h(0) : #x7 = SL_w_c_h
    GOSUB ht_stripped
    CHECK "nested removed" @617, t_in
    #x4 = VARPTR str_a_a_h(0) : #x5 = SL_a_a_h : #x6 = VARPTR str_w_a_h(0) : #x7 = SL_w_a_h
    GOSUB ht_stripped
    CHECK "unclosed < kept" @618, t_in
    #x4 = VARPTR str_a_d_h(0) : #x5 = SL_a_d_h : #x6 = VARPTR str_w_a_h(0) : #x7 = SL_w_a_h
    GOSUB ht_stripped
    CHECK "< before tag" @619, t_in
    GOSUB net_close
    CHECK "strip close" @622, fn_ok
END

test_network_http_put_plain: PROCEDURE
    SECTION "HTTP PUT plain"
    nw_mode = 8 : nw_trans = 0
    #fn_src = VARPTR str_put_url(0) : #cs_len = SL_put_url
    GOSUB net_open
    CHECK "open PUT" @639, fn_ok
    #fn_src = VARPTR str_put_plain_h(0) : #cp_n = SL_put_plain_h
    GOSUB net_http_post
    CHECK "http_put ok" @646, fn_ok
    GOSUB ht_read
    CHECK "read after PUT" @656, #rd_n > 0
    GOSUB net_close
    CHECK "close PUT" @663, fn_ok
END

test_network_close_failure_sticks: PROCEDURE
    SECTION "rejected PUT close"
    nw_mode = 8 : nw_trans = 0
    #fn_src = VARPTR str_forbid_url(0) : #cs_len = SL_forbid_url
    GOSUB net_open
    CHECK "open 403 PUT" @685, fn_ok
    #fn_src = VARPTR str_put_forbid_h(0) : #cp_n = SL_put_forbid_h
    GOSUB net_http_post
    CHECK "put ok" @687, fn_ok
    GOSUB net_close
    CHECK "1st close bad" @690, fn_ok = 0
    GOSUB net_close
    CHECK "2nd close bad" @692, fn_ok = 0
    GOSUB net_status
    CHECK "status 212" @698, nw_err = 212
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_devspec(0) : #cs_len = SL_devspec
    GOSUB net_open
    CHECK "next open ok" @702, fn_ok
    GOSUB net_close
    CHECK "its close ok" @704, fn_ok
END

test_network_http_put_delete: PROCEDURE
    SECTION "HTTP PUT and DELETE"
    nw_mode = $0E : nw_trans = 0
    #fn_src = VARPTR str_put_url(0) : #cs_len = SL_put_url
    GOSUB net_open
    CHECK "open PUT hdr" @721, fn_ok
    nw_a = 3 : GOSUB net_http_mode
    CHECK "start headers" @728, fn_ok
    #fn_src = VARPTR str_hdr_ctype_h(0) : #cs_len = SL_hdr_ctype_h : GOSUB ht_header
    CHECK "add header" @730, fn_ok
    nw_a = 0 : GOSUB net_http_mode
    CHECK "end headers" @732, fn_ok
    #fn_src = VARPTR str_put_body_h(0) : #cp_n = SL_put_body_h
    GOSUB net_http_post
    CHECK "http_put ok" @739, fn_ok
    GOSUB net_json_parse
    CHECK "json parse echo" @748, fn_ok
    #fn_src = VARPTR str_q_jtest_h(0) : #cs_len = SL_q_jtest_h
    GOSUB net_json_query
    #x0 = 0
    IF jq_bad = 0 THEN
        IF #jq_n > 0 THEN
            #sa = NB_BUF : #sb = VARPTR str_v_fjput_h(0) : #sn = SL_v_fjput_h + 1 : GOSUB str_eq
            #x0 = t_same
        END IF
    END IF
    CHECK "got JSON body" @751, #x0
    #fn_src = VARPTR str_q_ctype_h(0) : #cs_len = SL_q_ctype_h
    GOSUB net_json_query
    #x0 = 0
    IF jq_bad = 0 THEN
        IF #jq_n > 0 THEN
            #sa = NB_BUF : #sb = VARPTR str_v_appjson_h(0) : #sn = SL_v_appjson_h + 1 : GOSUB str_eq
            #x0 = t_same
        END IF
    END IF
    CHECK "got C-Type hdr" @755, #x0
    GOSUB net_close
    CHECK "close PUT" @763, fn_ok

    nw_mode = 9 : nw_trans = 0
    #fn_src = VARPTR str_delete_url(0) : #cs_len = SL_delete_url
    GOSUB net_open
    CHECK "http_delete ok" @770, fn_ok
    GOSUB ht_read
    CHECK "read DELETE" @778, #rd_n > 0
    GOSUB net_close
    CHECK "close DELETE" @781, fn_ok
END

test_html_parse_query: PROCEDURE
    SECTION "HTML parse/query"
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_html_url(0) : #cs_len = SL_html_url
    GOSUB net_open
    CHECK "open html" @53, fn_ok
    GOSUB net_html_parse
    CHECK "html parse" @56, fn_ok
    #fn_src = VARPTR str_q_h1_h(0) : #cs_len = SL_q_h1_h
    GOSUB net_json_query
    CHECK "h1 has text" @60, #jq_n > 0
    #sa = NB_BUF : #sb = VARPTR str_moby_h(0) : #sn = SL_moby_h : GOSUB str_has
    CHECK "h1 is Moby-Dick" @62, t_in
    GOSUB net_close
    CHECK "close html" @65, fn_ok
END

' Open the links page and parse it as HTML
ht_links: PROCEDURE
    nw_mode = 4 : nw_trans = 0
    #fn_src = VARPTR str_links_url(0) : #cs_len = SL_links_url
    GOSUB net_open
END

test_html_iterate: PROCEDURE
    SECTION "HTML iterate"
    GOSUB ht_links
    CHECK "open links" @84, fn_ok
    GOSUB net_html_parse
    CHECK "html parse" @87, fn_ok
    #x0 = 0                             ' matches
    hq_i = 0                            ' rounds
    hq_go = 1
    WHILE hq_go
        #fn_src = VARPTR str_q_a_h(0) : #cs_len = SL_q_a_h
        GOSUB net_json_query
        IF jq_bad THEN
            hq_go = 0
        ELSEIF #jq_n < 1 THEN
            hq_go = 0
        ELSE
            #x0 = #x0 + 1
            hq_i = hq_i + 1
            IF hq_i >= 8 THEN hq_go = 0
        END IF
    WEND
    CHECK "several anchors" @101, #x0 >= 2
    CHECK "iteration ends" @102, hq_i < 8
    GOSUB net_close
    CHECK "close links" @105, fn_ok
END

test_html_colon_selector: PROCEDURE
    SECTION "HTML colon selector"
    GOSUB ht_links
    CHECK "open links" @126, fn_ok
    GOSUB net_html_parse
    CHECK "html parse" @129, fn_ok
    #fn_src = VARPTR str_q_nth_h(0) : #cs_len = SL_q_nth_h
    GOSUB net_json_query
    CHECK "colon no error" @133, jq_bad = 0
    GOSUB net_close
    CHECK "close links" @137, fn_ok
END

test_html: PROCEDURE
    GOSUB test_network_http_get : IF t_abort THEN RETURN
    GOSUB test_network_http_304_completion : IF t_abort THEN RETURN
    GOSUB test_network_http_fixed_completion : IF t_abort THEN RETURN
    GOSUB test_network_http_chunked_completion : IF t_abort THEN RETURN
    GOSUB test_network_http_304 : IF t_abort THEN RETURN
    GOSUB test_network_http_get_nonblocking : IF t_abort THEN RETURN
    GOSUB test_network_http_post : IF t_abort THEN RETURN
    GOSUB test_network_http_post_large : IF t_abort THEN RETURN
    GOSUB test_network_http_headers : IF t_abort THEN RETURN
    GOSUB test_network_http_channel_mode : IF t_abort THEN RETURN
    GOSUB test_network_json : IF t_abort THEN RETURN
    GOSUB test_network_json_strip_tags : IF t_abort THEN RETURN
    GOSUB test_network_http_put_plain : IF t_abort THEN RETURN
    GOSUB test_network_http_put_delete : IF t_abort THEN RETURN
    GOSUB test_network_close_failure_sticks : IF t_abort THEN RETURN
    GOSUB test_html_parse_query : IF t_abort THEN RETURN
    GOSUB test_html_iterate : IF t_abort THEN RETURN
    GOSUB test_html_colon_selector
END
