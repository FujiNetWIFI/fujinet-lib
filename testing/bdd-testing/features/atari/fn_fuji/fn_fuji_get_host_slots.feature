Feature: IO library test - fn_fuji_get_host_slots

  This tests FN-FUJI fn_fuji_get_host_slots

  Scenario: execute _fuji_get_host_slots
    Given atari-fn-fuji simple test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fuji_get_host_slots.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple.s"
      And I create and load simple atari application
      And I write memory at $80 with $00
      And I set register A to $00
      And I set register X to $a0

     When I execute the procedure at _fuji_get_host_slots for no more than 60 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $f4
     And I expect to see DSTATS equal $40
     And I expect to see DBYTLO equal $00
     And I expect to see DBYTHI equal $01
     And I expect to see DAUX1 equal $00
     And I expect to see DAUX2 equal $00
     And I expect to see DBUFLO equal lo($a000)
     And I expect to see DBUFHI equal hi($a000)

     # verify BUS was called
     And I expect to see $80 equal 1
