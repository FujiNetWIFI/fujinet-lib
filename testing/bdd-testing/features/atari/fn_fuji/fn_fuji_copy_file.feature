Feature: IO library test - fn_fuji_copy_file

  This tests FN-FUJI fn_fuji_copy_file

  Scenario: execute _fn_fuji_copy_file
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fn_fuji_copy_file.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_bbw.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      # src, dst, spec
      And I write memory at t_b1 with $01
      And I write memory at t_b2 with $02
      And I write word at t_w3 with hex a000
      And I write word at t_fn with address _fn_fuji_copy_file
     When I execute the procedure at _init for no more than 120 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $fe
     And I expect to see DCOMND equal $d8
     And I expect to see DSTATS equal $80
     And I expect to see DBYTLO equal $00
     And I expect to see DBYTHI equal $01
     # src/dst are incremented by 1 for FN call
     And I expect to see DAUX1 equal $02
     And I expect to see DAUX2 equal $03
     And I expect to see DBUFLO equal lo($A000)
     And I expect to see DBUFHI equal hi($A000)

    # check BUS was called
    Then I expect to see $80 equal $01
