' parserr: tests/parserr.c
STR q_title "/slideshow/title"

' Open the spec at #fn_src/#cs_len for HTTP GET
pe_open: PROCEDURE
    nw_mode = 4 : nw_trans = 0
    GOSUB net_open
END

' Open the spec at #fn_src/#cs_len, close it by the same unit
pe_close_url: PROCEDURE
    GOSUB net_unit_of
    GOSUB net_close
END

test_parse_no_parser: PROCEDURE
    SECTION "PARSE, no channel"
    #fn_src = VARPTR str_json_url(0) : GOSUB pe_close_url
    GOSUB net_parse
    CHECK "bare parse bad" @94, fn_ok = 0
    GOSUB fuji_get_adapter_config
    CHECK "alive 1" @97, fn_ok
    GOSUB net_status
    CHECK "no proto 207" @100, nw_err = 207
END

test_parse_after_close: PROCEDURE
    SECTION "PARSE after CLOSE"
    #fn_src = VARPTR str_json_url(0) : #cs_len = SL_json_url
    GOSUB pe_open
    CHECK "open json" @120, fn_ok
    GOSUB net_json_parse
    CHECK "good json parse" @123, fn_ok
    GOSUB net_close
    CHECK "close json" @126, fn_ok
    GOSUB net_parse
    CHECK "post-close bad" @131, fn_ok = 0
    GOSUB fuji_get_adapter_config
    CHECK "alive 2" @132, fn_ok
    GOSUB net_status
    CHECK "closed is 207" @135, nw_err = 207
END

test_parse_parser_none: PROCEDURE
    SECTION "PARSE, PARSER_NONE"
    #fn_src = VARPTR str_json_url(0) : #cs_len = SL_json_url
    GOSUB pe_open
    CHECK "open json" @154, fn_ok
    nw_a = 0 : GOSUB net_set_parser
    CHECK "set none" @157, fn_ok
    GOSUB net_parse
    CHECK "no parser bad" @163, fn_ok = 0
    GOSUB net_status
    GOSUB net_close
    CHECK "close json" @170, fn_ok
END

test_json_parse_malformed: PROCEDURE
    SECTION "JSON of non-JSON"
    #fn_src = VARPTR str_html_url(0) : #cs_len = SL_html_url
    GOSUB pe_open
    CHECK "open html" @189, fn_ok
    GOSUB net_json_parse
    CHECK "json/html bad" @192, fn_ok = 0
    GOSUB net_status
    CHECK "status is 213" @196, nw_err = 213
    GOSUB fuji_get_adapter_config
    CHECK "alive 3" @199, fn_ok
    nw_unit = 1
    GOSUB net_close
    CHECK "close html" @202, fn_ok
END

test_json_parse_empty_body: PROCEDURE
    SECTION "JSON of empty body"
    #fn_src = VARPTR str_empty_url(0) : #cs_len = SL_empty_url
    GOSUB pe_open
    CHECK "open 204" @222, fn_ok
    GOSUB net_json_parse
    CHECK "empty json bad" @225, fn_ok = 0
    GOSUB net_status
    CHECK "empty is 213" @229, nw_err = 213
    GOSUB net_close
    CHECK "close 204" @233, fn_ok
END

test_html_parse_empty_body: PROCEDURE
    SECTION "HTML of empty body"
    #fn_src = VARPTR str_empty_url(0) : #cs_len = SL_empty_url
    GOSUB pe_open
    CHECK "open 204" @253, fn_ok
    GOSUB net_html_parse
    CHECK "empty html bad" @256, fn_ok = 0
    GOSUB net_status
    CHECK "empty is 144" @260, nw_err = 144
    GOSUB fuji_get_adapter_config
    CHECK "alive 4" @263, fn_ok
    nw_unit = 1
    GOSUB net_close
    CHECK "close 204" @266, fn_ok
END

test_parse_error_not_sticky: PROCEDURE
    SECTION "error not sticky"
    #fn_src = VARPTR str_html_url(0) : #cs_len = SL_html_url
    GOSUB pe_open
    CHECK "open html" @286, fn_ok
    GOSUB net_json_parse
    GOSUB net_status
    CHECK "poisoned 213" @289, nw_err = 213
    GOSUB net_close
    CHECK "close html" @292, fn_ok

    #fn_src = VARPTR str_json_url(0) : #cs_len = SL_json_url
    GOSUB pe_open
    CHECK "open json" @297, fn_ok
    GOSUB net_json_parse
    CHECK "good parse 2" @300, fn_ok
    GOSUB net_status
    #x0 = 0
    IF nw_err = 1 THEN #x0 = 1
    IF nw_err = 136 THEN #x0 = 1
    CHECK "clean status" @303, #x0

    #fn_src = VARPTR str_q_title(0) : #cs_len = SL_q_title
    GOSUB net_json_query
    #x0 = 0
    IF jq_bad = 0 THEN
        IF #jq_n > 0 THEN #x0 = 1
    END IF
    CHECK "query has data" @309, #x0
    GOSUB net_close
    CHECK "close json" @312, fn_ok
END

test_query_after_failed_parse: PROCEDURE
    SECTION "query, bad parse"
    #fn_src = VARPTR str_html_url(0) : #cs_len = SL_html_url
    GOSUB pe_open
    CHECK "open html" @331, fn_ok
    GOSUB net_json_parse
    #fn_src = VARPTR str_q_title(0) : #cs_len = SL_q_title
    GOSUB net_json_query
    #x0 = 0
    IF jq_bad THEN #x0 = 1
    IF #jq_n = 0 THEN #x0 = 1
    CHECK "no data back" @342, #x0
    GOSUB net_status
    CHECK "still 213" @347, nw_err = 213
    GOSUB fuji_get_adapter_config
    CHECK "alive 5" @349, fn_ok
    nw_unit = 1
    GOSUB net_close
    CHECK "close html" @352, fn_ok
END

test_query_no_channel: PROCEDURE
    SECTION "query, no channel"
    #fn_src = VARPTR str_json_url(0) : GOSUB pe_close_url
    #fn_src = VARPTR str_q_title(0) : #cs_len = SL_q_title
    GOSUB net_json_query
    #x0 = 0
    IF jq_bad THEN #x0 = 1
    IF #jq_n = 0 THEN #x0 = 1
    CHECK "no channel 0" @376, #x0
    CHECK "no channel err" @377, jq_bad
    GOSUB net_status
    CHECK "bus in sync" @380, nw_err = 207
    GOSUB fuji_get_adapter_config
    CHECK "alive 6" @382, fn_ok
END

test_set_parser_invalid_mode: PROCEDURE
    SECTION "invalid parser mode"
    #fn_src = VARPTR str_json_url(0) : #cs_len = SL_json_url
    GOSUB pe_open
    CHECK "open json" @400, fn_ok
    nw_a = 4 : GOSUB net_set_parser
    CHECK "mode 4 bad" @406, fn_ok = 0
    GOSUB fuji_get_adapter_config
    CHECK "alive 7" @407, fn_ok
    nw_a = 1 : GOSUB net_set_parser
    CHECK "json still ok" @411, fn_ok
    GOSUB net_close
    CHECK "close json" @413, fn_ok
END

test_set_parameter_errors: PROCEDURE
    SECTION "SET_PARAMETER errors"
    #fn_src = VARPTR str_json_url(0) : GOSUB pe_close_url
    nw_a = 0 : GOSUB net_set_query_param
    CHECK "no parser bad" @433, fn_ok = 0
    GOSUB fuji_get_adapter_config
    CHECK "alive 8" @435, fn_ok

    #fn_src = VARPTR str_json_url(0) : #cs_len = SL_json_url
    GOSUB pe_open
    CHECK "open json" @438, fn_ok
    nw_a = 0 : GOSUB net_set_query_param
    CHECK "no parser 2 bad" @442, fn_ok = 0
    nw_a = 1 : GOSUB net_set_parser
    CHECK "set json" @445, fn_ok
    nw_a = $20 : GOSUB net_set_query_param
    CHECK "reserved bad" @448, fn_ok = 0
    nw_a = 2 : GOSUB net_set_query_param
    CHECK "valid flag ok" @451, fn_ok
    GOSUB net_close
    CHECK "close json" @454, fn_ok
END

test_parserr: PROCEDURE
    GOSUB test_parse_no_parser : IF t_abort THEN RETURN
    GOSUB test_parse_after_close : IF t_abort THEN RETURN
    GOSUB test_parse_parser_none : IF t_abort THEN RETURN
    GOSUB test_json_parse_malformed : IF t_abort THEN RETURN
    GOSUB test_json_parse_empty_body : IF t_abort THEN RETURN
    GOSUB test_html_parse_empty_body : IF t_abort THEN RETURN
    GOSUB test_parse_error_not_sticky : IF t_abort THEN RETURN
    GOSUB test_query_after_failed_parse : IF t_abort THEN RETURN
    GOSUB test_query_no_channel : IF t_abort THEN RETURN
    GOSUB test_set_parser_invalid_mode : IF t_abort THEN RETURN
    GOSUB test_set_parameter_errors
END
