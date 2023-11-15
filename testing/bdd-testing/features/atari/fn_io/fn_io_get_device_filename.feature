Feature: IO library test - fn_io_get_device_filename

  This tests FN-IO fn_io_get_device_filename

  Scenario Outline: execute _fn_io_get_device_filename
    Given atari-fn-io application test setup
      And I add common atari-io files
      And I add atari src file "fn_io/fn_io_get_device_filename.s"
      And I add file for compiling "features/atari/fn_io/test-apps/test_bw.s"
      And I add file for compiling "features/atari/fn_io/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $00

      And I write memory at t_b1 with <slot_offset>
      And I write word at t_w2 with hex a000
      And I write word at t_fn with address _fn_io_get_device_filename

     When I execute the procedure at _init for no more than 90 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $da
     And I expect to see DSTATS equal $40
     And I expect to see DBYTLO equal $00
     And I expect to see DBYTHI equal $01
     And I expect to see DAUX1 equal <slot_offset>
     And I expect to see DAUX2 equal $00
     And I expect to see DBUFLO equal lo($A000)
     And I expect to see DBUFHI equal hi($A000)

     # verify BUS was called
     And I expect to see $80 equal 1

  Examples:
  | slot_offset | comment           |
  | $07         | normal slots 0-7  |
  | $1a         | cas slot          |