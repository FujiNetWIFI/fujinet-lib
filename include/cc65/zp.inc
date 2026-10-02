; use sreg, regsave areas of ZP as usable tmpX vars
; DEPENDS ON START OF ZP IN CFG!

.include "zeropage.inc"

; sreg
tmp5    := sreg
tmp6    := sreg+1
; regsave
tmp7    := regsave
tmp8    := regsave+1
tmp9    := regsave+2
tmp10   := regsave+3
