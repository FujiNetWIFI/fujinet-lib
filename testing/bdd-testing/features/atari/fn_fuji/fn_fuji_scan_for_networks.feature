Feature: IO library test - fn_fuji_scan_for_networks

  This tests FN-FUJI fn_fuji_scan_for_networks

  Scenario Outline: execute _fuji_scan_for_networks
    Given atari-fn-fuji simple test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fuji_scan_for_networks.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-dbuflo1.s"
      And I create and load simple atari application
      And I write memory at $80 with $00

     When I set register A to $aa
      And I write memory at t_v with <networks>
      And I execute the procedure at _fuji_scan_for_networks for no more than 65 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $fd
     And I expect to see DSTATS equal $40
     And I expect to see DBYTLO equal $04
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $00
     And I expect to see DAUX2 equal $00
     # using tmp9, which in cc65 is ZP address $88
     And I expect to see DBUFLO equal lo($88)
     And I expect to see DBUFHI equal hi($88)

     And I expect register A equal <networks>
     And I expect register X equal 0

    Examples:
    | networks |
    | 0        |
    | 1        |
    | 10       |