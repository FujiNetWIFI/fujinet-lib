        .export         sp_rw_common

        .import         _sp_cmdlist
        .import         _sp_payload

        .import         popa

        .include        "sp.inc"

; expects A/X to hold LEN value
; and software stack to hold dest

.proc sp_rw_common

        sta     _sp_cmdlist+4                   ; sp_cmdlist[4] = len & 0xFF;
        stx     _sp_cmdlist+5                   ; sp_cmdlist[5] = (len >> 8) & 0xFF;

        jsr     popa
        sta     _sp_cmdlist+1                   ; sp_cmdlist[1] = dest;
        lda     #<(_sp_payload)
        sta     _sp_cmdlist+2                   ; sp_cmdlist[2] = payload_address & 0xFF;
        lda     #>(_sp_payload)
        sta     _sp_cmdlist+3                   ; sp_cmdlist[3] = payload_address >> 8;
        rts

.endproc