; stub BUS
  .include    "macros.inc"
  .include    "fujinet-io.inc"
  .include    "device.inc"
  .export     ac

; short copy <$7F bytes (uses bpl), using Y to index
.macro copy_y arg1, arg2, arg3
    .local L1
    ldy arg1-1
L1: lda arg2
    sta arg3
    dey
    bpl L1
.endmacro

  .segment "BUS"
  .org BUS
  ; Emulate BUS call by copying information directly

  ; copy test data into struct
  copy_y #11, {t_ssid,y},          {ac+AdapterConfigExtended::ssid, y}
  copy_y #14, {t_hostname,y},      {ac+AdapterConfigExtended::hostname,y}
  copy_y  #2, {t_local_ip,y},      {ac+AdapterConfigExtended::localIP,y}
  copy_y  #2, {t_gateway,y},       {ac+AdapterConfigExtended::gateway,y}
  copy_y  #2, {t_netmask,y},       {ac+AdapterConfigExtended::netmask,y}
  copy_y  #3, {t_dns_ip,y},        {ac+AdapterConfigExtended::dnsIP,y}
  copy_y  #6, {t_mac_address,y},   {ac+AdapterConfigExtended::macAddress,y}
  copy_y  #5, {t_bssid,y},         {ac+AdapterConfigExtended::bssid,y}
  copy_y #14, {t_fn_version,y},    {ac+AdapterConfigExtended::fn_version,y}
  copy_y #15, {t_s_local_ip,y},    {ac+AdapterConfigExtended::sLocalIP,y}
  copy_y #15, {t_s_gateway,y},     {ac+AdapterConfigExtended::sGateway,y}
  copy_y #11, {t_s_netmask,y},     {ac+AdapterConfigExtended::sNetmask,y}
  copy_y #15, {t_s_dns_ip,y},      {ac+AdapterConfigExtended::sDnsIP,y}
  copy_y #17, {t_s_mac_address,y}, {ac+AdapterConfigExtended::sMacAddress,y}
  copy_y #17, {t_s_bssid,y},       {ac+AdapterConfigExtended::sBssid,y}

  ; copy the test adapterconfig to the caller's buffer.
  mwa IO_DCB::dbuflo, $80  ; copy DBUF pointers into ZP

  ldy #0
: mva {ac, y}, {($80), y}
  iny
  cpy #.sizeof(AdapterConfigExtended)-1
  bne :-

  rts

; locations for test to set values
t_ssid:          .byte "ssid name!!"
t_hostname:      .byte "the 'hostname'"
t_local_ip:      .byte "ip"
t_gateway:       .byte "gw"
t_netmask:       .byte "nm"
t_dns_ip:        .byte "dns"
t_mac_address:   .byte "macadd"
t_bssid:         .byte "bssid"
t_fn_version:    .byte "version string"
t_s_local_ip:    .byte "100.100.100.100"
t_s_gateway:     .byte "101.101.101.101"
t_s_netmask:     .byte "99.99.99.99"
t_s_dns_ip:      .byte "102.102.102.102"
t_s_mac_address: .byte "00:11:22:33:44:55"
t_s_bssid:       .byte "aa:bb:cc:dd:ee:ff"

; location to store our stubbed return
ac:            .tag AdapterConfigExtended
