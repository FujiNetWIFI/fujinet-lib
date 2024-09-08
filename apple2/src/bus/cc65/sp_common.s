        .export         sp_common_params

        .import         _sp_cmdlist
        .import         _sp_payload

        .import         popa

        .include        "sp.inc"

; set A to the code to store in sp_cmdlist[4]
; the dest param is expected to be on s/w stack as 1 byte

.proc sp_common_params

        sta     _sp_cmdlist+4                   ; sp_cmdlist[4] = <CODE>
        jsr     popa
        sta     _sp_cmdlist+1                   ; sp_cmdlist[1] = dest;
        lda     #<(_sp_payload)
        sta     _sp_cmdlist+2                   ; sp_cmdlist[2] = payload_address & 0xFF;
        lda     #>(_sp_payload)
        sta     _sp_cmdlist+3                   ; sp_cmdlist[3] = payload_address >> 8;
        rts

.endproc