Feature: IO library test - fn_io_mount_all

  This tests FN-IO fn_io_mount_all

  Scenario: execute _fn_io_mount_all
    Given atari-fn-io application test setup
      And I add common atari-io files
      And I add atari src file "fn_io/fn_io_mount_all.s"
      And I add file for compiling "features/atari/fn_io/test-apps/test_no_args.s"
      And I add file for compiling "features/atari/fn_io/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      And I write word at t_fn with address _fn_io_mount_all
     When I execute the procedure at _init for no more than 70 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $d7
     And I expect to see DSTATS equal $00
     And I expect to see DBYTLO equal $00
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $00
     And I expect to see DAUX2 equal $00
     And I expect to see DBUFLO equal $00
     And I expect to see DBUFHI equal $00

    # check BUS was called
    Then I expect to see $80 equal $01
