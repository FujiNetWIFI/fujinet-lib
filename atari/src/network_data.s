        .export     fn_open_mode_table
        .export     fn_open_trans_table

; index into these to get the currently open channel's mode/trans, set when open is called
fn_open_mode_table:     .res 8
fn_open_trans_table:    .res 8
