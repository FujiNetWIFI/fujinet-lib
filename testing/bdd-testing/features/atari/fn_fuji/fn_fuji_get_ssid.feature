Feature: IO library test - fn_fuji_get_ssid

  This tests FN-FUJI fn_fuji_get_ssid

  Scenario: execute _fn_fuji_get_ssid
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fn_fuji_get_ssid.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_w.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-netconfig.s"
      And I create and load atari application
      And I write word at t_w1 with hex a000
      And I write word at t_fn with address _fn_fuji_get_ssid
      # And I print memory from BUS to BUS+192

     When I execute the procedure at _init for no more than 540 instructions
      And I print memory from $A000 to $A000+98

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $fe
     And I expect to see DSTATS equal $40
     And I expect to see DBYTLO equal 97
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $00
     And I expect to see DAUX2 equal $00
     And I expect to see DBUFLO equal lo($A000)
     And I expect to see DBUFHI equal hi($A000)

    # test the ssid was copied into struct
     And I hex dump memory between $A000 and $A008
    Then property "test.BDD6502.lastHexDump" must contain string "yourssid"

    # test the password was copied into struct
     And I hex dump memory between $A021 and $A029
    Then property "test.BDD6502.lastHexDump" must contain string "password"
