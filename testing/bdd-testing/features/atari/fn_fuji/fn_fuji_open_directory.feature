Feature: IO library test - fn_fuji_open_directory

  This tests FN-FUJI fn_fuji_open_directory

  Scenario: execute _fuji_open_directory with filter and path
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fuji_open_directory.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_bw.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      # host_slot, buffer
      And I write memory at t_b1 with $03
      And I write word at t_w2 with hex a000
      And I write word at t_fn with address _fuji_open_directory
     When I execute the procedure at _init for no more than 105 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $f7
     And I expect to see DSTATS equal $80
     And I expect to see DBYTLO equal $00
     And I expect to see DBYTHI equal $01
     And I expect to see DAUX1 equal 3
     And I expect to see DAUX2 equal $00
     And I expect to see DBUFLO equal lo($a000)
     And I expect to see DBUFHI equal hi($a000)

    # check BUS was called
    Then I expect to see $80 equal $01
