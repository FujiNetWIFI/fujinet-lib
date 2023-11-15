Feature: IO library test - fn_io_get_hsio_index

  This tests FN-IO fn_io_get_hsio_index

  Scenario: execute _fn_io_get_hsio_index
    Given atari-fn-io application test setup
      And I add common atari-io files
      And I add atari src file "fn_io/fn_io_get_hsio_index.s"
      And I add file for compiling "features/atari/fn_io/test-apps/test_no_args.s"
      And I add file for compiling "features/atari/fn_io/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      # code will only use tmp7, just proving that tmp7+1 doesn't affect result here, by setting tmp7 and tmp7+1
      And I write word at $86 with hex 1234
      And I write word at t_fn with address _fn_io_get_hsio_index
     When I execute the procedure at _init for no more than 85 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DCOMND equal $3f
     And I expect to see DSTATS equal $40
     # location of tmp7, or regsave
     And I expect to see DBUFLO equal $86
     And I expect to see DBUFHI equal 00
     And I expect to see DTIMLO equal $0f
     And I expect to see DBYTLO equal 1
     And I expect to see DBYTHI equal 0
     And I expect to see DAUX1 equal 0
     And I expect to see DAUX2 equal 0

    # check BUS was called
    Then I expect to see $80 equal $01
    # and the A is from tmp7, X is 0 (hi byte)
     And I expect register A equal $34
     And I expect register X equal $00

