Feature: IO library test - fn_io_base64_encode_compute

  This tests FN-IO fn_io_base64_encode_compute

  Scenario: execute _fn_io_base64_encode_compute
    Given atari-fn-io application test setup
      And I add common atari-io files
      And I add atari src file "fn_io/fn_io_base64_xxcode_compute.s"
      And I add file for compiling "features/atari/fn_io/test-apps/test_no_args.s"
      And I add file for compiling "features/atari/fn_io/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      And I write word at t_fn with address _fn_io_base64_encode_compute
     When I execute the procedure at _init for no more than 85 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DCOMND equal $cf
     And I expect to see DSTATS equal 0
     And I expect to see DBUFLO equal 0
     And I expect to see DBUFHI equal 0
     And I expect to see DTIMLO equal 3
     And I expect to see DBYTLO equal 0
     And I expect to see DBYTHI equal 0
     And I expect to see DAUX1 equal 0
     And I expect to see DAUX2 equal 0

    # check BUS was called
    Then I expect to see $80 equal $01
