; Smart Port definitions

SP_CMD_STATUS           := 0
SP_CMD_CONTROL          := 4
SP_CMD_OPEN             := 6
SP_CMD_CLOSE            := 7
SP_CMD_READ             := 8
SP_CMD_WRITE            := 9

; FUJI versions don't send the network unit
SP_STATUS_PARAM_COUNT   := 3
SP_CONTROL_PARAM_COUNT  := 3
SP_OPEN_PARAM_COUNT     := 0
SP_CLOSE_PARAM_COUNT    := 0
SP_READ_PARAM_COUNT     := 4
SP_WRITE_PARAM_COUNT    := 4

; NETWORK versions have an extra byte for the network unit
SP_STATUS_PARAM_COUNT_NW   := 4
SP_CONTROL_PARAM_COUNT_NW  := 4
SP_OPEN_PARAM_COUNT_NW     := 1
SP_CLOSE_PARAM_COUNT_NW    := 1
SP_READ_PARAM_COUNT_NW     := 5
SP_WRITE_PARAM_COUNT_NW    := 5

SP_ERR_OK               := $00
SP_ERR_BAD_CMD          := $01
SP_ERR_BAD_PCNT         := $02
SP_ERR_BUS_ERR          := $06
SP_ERR_BAD_UNIT         := $11
SP_ERR_BAD_CTRL         := $21
SP_ERR_BAD_CTRL_PARM    := $22
SP_ERR_IO_ERROR         := $27
SP_ERR_NO_DRIVE         := $28
SP_ERR_NO_WRITE         := $2B
SP_ERR_BAD_BLOCK        := $2D
SP_ERR_DISK_SW          := $2E
SP_ERR_OFFLINE          := $2F
SP_ERR_DEV_SPEC0        := $30
SP_ERR_DEV_SPECF        := $3F
; SP_ERR_RESERVED $40-$4F
SP_ERR_NON_FATAL50      := $50
SP_ERR_NON_FATAL7F      := $7F

SP_PAYLOAD_SIZE         := 512