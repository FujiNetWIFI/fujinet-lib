Feature: IO library test - fn_fuji_get_scan_result

  This tests FN-FUJI fn_fuji_get_scan_result

  Scenario: execute _fuji_get_scan_result
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fuji_get_scan_result.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_bw.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-ssid-info.s"
      And I create and load atari application
      And I write memory at t_b1 with 5
      And I write word at t_w2 with hex a000
      And I write word at t_fn with address _fuji_get_scan_result

     When I execute the procedure at _init for no more than 290 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $fc
     And I expect to see DSTATS equal $40
     And I expect to see DBYTLO equal 34
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $05
     And I expect to see DAUX2 equal $00
     And I expect to see DBUFLO equal lo($A000)
     And I expect to see DBUFHI equal hi($A000)

    # test the ssid was copied into struct
     And I hex dump memory between $A000 and $A008
    Then property "test.BDD6502.lastHexDump" must contain string "ssidtime"

    # test the rssi was copied into struct
     And I hex dump memory between $A021 and $A022
    Then property "test.BDD6502.lastHexDump" must contain string ": 69 :"
