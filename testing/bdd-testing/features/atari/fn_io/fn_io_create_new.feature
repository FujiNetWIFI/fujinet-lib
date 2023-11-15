Feature: IO library test - fn_io_create_new

  This tests FN-IO fn_io_create_new

  Scenario: execute _fn_io_create_new
    Given atari-fn-io application test setup
      And I add common atari-io files
      And I add atari src file "fn_io/fn_io_create_new.s"
      And I add file for compiling "features/atari/fn_io/test-apps/test_w.s"
      And I add file for compiling "features/atari/fn_io/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      And I write word at t_w1 with hex c000
      And I write word at t_fn with address _fn_io_create_new
     When I execute the procedure at _init for no more than 80 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $fe
     And I expect to see DCOMND equal $e7
     And I expect to see DSTATS equal $80
     And I expect to see DBYTLO equal $06
     And I expect to see DBYTHI equal $01
     And I expect to see DAUX1 equal $00
     And I expect to see DAUX2 equal $00
     And I expect to see DBUFLO equal lo($c000)
     And I expect to see DBUFHI equal hi($c000)

     # check BUS was called
     And I expect to see $80 equal $01
