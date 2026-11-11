' harness.bas -- the fujinet-lib test harness (tests/harness.c) on a 20x12
' screen. Row 0 header, rows 1-10 scrolling log, row 11 section.
' A failed check aborts the run, as in the C harness.

    CONST C_BLACK  = 0
    CONST C_RED    = 2
    CONST C_GREEN  = 5
    CONST C_YELLOW = 6
    CONST C_WHITE  = 7

    CONST RES_BASE = $9B00      ' result block read by tools/results.py

    DIM #t_run, #t_skip, #t_fail
    DIM t_ok, t_abort
    DIM #t_id
    DIM #t_cl
    DIM #t_nm, #t_w, #t_c, t_pos
    DIM #t_base                 ' BACKTAB index of the name's first column

    DIM #x0, #x1, #x2, #x3, #x4, #x5, #x6, #x7     ' scratch shared by every test
    DIM #rv, #rlo, #rhi, t_in
    DIM #sa, #sb, #sn, t_same

' t_in = (#rlo <= #rv <= #rhi)
in_range: PROCEDURE
    t_in = 0
    IF #rv >= #rlo THEN
        IF #rv <= #rhi THEN t_in = 1
    END IF
END

' t_same = the #sn bytes at #sa (RAM) equal the #sn bytes at #sb (ROM or RAM)
str_eq: PROCEDURE
    t_same = 1
    IF #sn > 0 THEN
        FOR #t_w = 0 TO #sn - 1
            IF (PEEK(#sa + #t_w) AND 255) <> (PEEK(#sb + #t_w) AND 255) THEN t_same = 0
        NEXT #t_w
    END IF
END

' Scroll the log (rows 1-10) up one line and clear the bottom log row.
log_scroll: PROCEDURE
    FOR #t_w = 20 TO 199
        #BACKTAB(#t_w) = #BACKTAB(#t_w + 20)
    NEXT #t_w
    FOR #t_w = 200 TO 219
        #BACKTAB(#t_w) = 0
    NEXT #t_w
END

' Draw check name #t_id at #t_base (up to 15 columns), in colour #t_c.
log_name: PROCEDURE
    #t_nm = VARPTR nm_tab(0) + #t_id * 8
    FOR t_pos = 0 TO 7
        #t_w = PEEK(#t_nm + t_pos)
        #BACKTAB(#t_base + t_pos * 2) = ((#t_w AND 255) - 32) * 8 + #t_c
        IF t_pos < 7 THEN
            #BACKTAB(#t_base + 1 + t_pos * 2) = ((#t_w / 256) - 32) * 8 + #t_c
        END IF
    NEXT t_pos
END

' Record a check result: t_ok, #t_id, #t_cl. Sets t_abort on a failure.
check: PROCEDURE
    #t_run = #t_run + 1
    GOSUB log_scroll
    IF t_ok THEN
        PRINT AT 200 COLOR C_GREEN, "PASS"
        #t_c = C_GREEN
        #t_base = 205
        GOSUB log_name
        GOSUB header_counts
        RETURN
    END IF
    #t_fail = #t_fail + 1
    t_abort = 1
    PRINT AT 200 COLOR C_RED, "FAIL"
    #t_c = C_RED
    #t_base = 205
    GOSUB log_name
    GOSUB log_scroll
    PRINT AT 200 COLOR C_RED, "LINE ", <.4>#t_cl
    GOSUB log_scroll
    PRINT AT 200 COLOR C_RED, "*** ABORTED ***"
END

' A check this mailbox cannot carry out: logged, counted as skipped.
skip_log: PROCEDURE
    #t_skip = #t_skip + 1
    GOSUB log_scroll
    PRINT AT 200 COLOR C_WHITE, "SKIP"
    #t_c = C_WHITE
    #t_base = 205
    GOSUB log_name
    GOSUB header_counts
END

header_counts: PROCEDURE
    PRINT AT 13 COLOR C_YELLOW, <.3>#t_run
    PRINT AT 16 COLOR C_YELLOW, "S", <.2>#t_skip
END

' Begin a program: clear the screen, zero the counters, draw the title.
begin_testing: PROCEDURE
    #t_run = 0 : #t_skip = 0 : #t_fail = 0 : t_abort = 0
    FOR #t_w = 0 TO 239
        #BACKTAB(#t_w) = 0
    NEXT #t_w
    PRINT AT 0 COLOR C_WHITE, "                    "
    POKE RES_BASE + 1, 0
END

' Breakpoint target for tools/run.sh: the result block is complete.
results_ready: PROCEDURE
END

' Print the results screen and the result block, then wait for ENTER.
end_testing: PROCEDURE
    FOR #t_w = 20 TO 239
        #BACKTAB(#t_w) = 0
    NEXT #t_w
    IF t_abort THEN
        PRINT AT 20 COLOR C_RED, "ABORTED AFTER FIRST"
        PRINT AT 40 COLOR C_RED, "FAILURE"
        #t_c = C_RED
        #t_base = 60
        GOSUB log_name
        PRINT AT 80 COLOR C_RED, "C LINE ", <.4>#t_cl
    ELSE
        PRINT AT 20 COLOR C_GREEN, "ALL TESTS PASSED"
    END IF
    PRINT AT 120 COLOR C_WHITE, "PASS ", <.4>(#t_run - #t_fail)
    PRINT AT 140 COLOR C_WHITE, "SKIP ", <.4>#t_skip
    PRINT AT 160 COLOR C_WHITE, "FAIL ", <.4>#t_fail
    PRINT AT 180 COLOR C_WHITE, "TOTAL ", <.4>#t_run
    PRINT AT 200 COLOR C_WHITE, "PRESS ENTER"
    POKE RES_BASE + 0, $A5
    POKE RES_BASE + 2, #t_run AND 255
    POKE RES_BASE + 3, #t_run / 256
    POKE RES_BASE + 4, #t_skip AND 255
    POKE RES_BASE + 5, #t_skip / 256
    POKE RES_BASE + 6, #t_fail AND 255
    POKE RES_BASE + 7, #t_fail / 256
    POKE RES_BASE + 8, #t_cl AND 255
    POKE RES_BASE + 9, #t_cl / 256
    POKE RES_BASE + 1, 1
    GOSUB results_ready
    WHILE CONT.KEY <> 11
        WAIT
    WEND
    WHILE CONT.KEY <> 12
        WAIT
    WEND
END
