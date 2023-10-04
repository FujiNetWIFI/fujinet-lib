        .export     fn_open_mode

.bss

; apple specific currently open network device's mode. atari keeps 8. need to review if this is correct.
; it's tied to the fact apple2 only has 1 network device.
fn_open_mode:   .res 1
