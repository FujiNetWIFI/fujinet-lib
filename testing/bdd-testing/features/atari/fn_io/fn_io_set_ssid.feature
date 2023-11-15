Feature: IO library test - fn_io_set_ssid

  This tests FN-IO fn_io_set_ssid

  Scenario: execute _fn_io_set_ssid
    Given atari-fn-io simple test setup
      And I add common atari-io files
      And I add atari src file "fn_io/fn_io_set_ssid.s"
      And I add file for compiling "features/atari/fn_io/stubs/bus-simple.s"
      And I create and load simple atari application
      And I set register A to $00
      And I set register X to $A0
      And I write memory at $80 with $00

     When I execute the procedure at _fn_io_set_ssid for no more than 60 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $fb
     And I expect to see DSTATS equal $80
     And I expect to see DBYTLO equal 97
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $01
     And I expect to see DBUFLO equal lo($A000)
     And I expect to see DBUFHI equal hi($A000)

     # prove we called bus
     And I expect to see $80 equal $01