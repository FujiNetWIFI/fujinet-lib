Feature: IO library test - fn_fuji_status

  This tests FN-FUJI fn_fuji_status

  Scenario: execute _fn_fuji_status
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fn_fuji_status.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_w.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      And I write word at t_w1 with hex 3355
      And I write word at t_fn with address _fn_fuji_status
     When I execute the procedure at _init for no more than 120 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DCOMND equal $53
     And I expect to see DSTATS equal $40
     And I expect to see DBUFLO equal $55
     And I expect to see DBUFHI equal $33
     And I expect to see DTIMLO equal $0f
     And I expect to see DBYTLO equal 4
     And I expect to see DBYTHI equal 0
     And I expect to see DAUX1 equal 0
     And I expect to see DAUX2 equal 0

    # check BUS was called
    Then I expect to see $80 equal $01

