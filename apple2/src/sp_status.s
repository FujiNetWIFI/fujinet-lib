        .export     _sp_statusX


.proc _sp_statusX
        lda     #$20
        sta     spJsr


spJsr:


.endproc

.data
sp_cmdlistX:     .res 10
