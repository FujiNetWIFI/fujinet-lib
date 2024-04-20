Feature: IO library test - fn_fuji_appkey_write

  This tests FN-FUJI fn_fuji_appkey_write

  Scenario: execute _fuji_appkey_write
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fuji_appkey_write.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_ww.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      And I write word at t_w1 with hex f00d
      And I write word at t_w2 with hex a000
      And I write word at t_fn with address _fuji_appkey_write
     When I execute the procedure at _init for no more than 120 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DCOMND equal $de
     And I expect to see DSTATS equal $80
     And I expect to see DBUFLO equal lo($a000)
     And I expect to see DBUFHI equal hi($a000)
     And I expect to see DTIMLO equal $0f
     And I expect to see DBYTLO equal 64
     And I expect to see DBYTHI equal 0
     And I expect to see DAUX1 equal $0d
     And I expect to see DAUX2 equal $f0

    # check BUS was called
    Then I expect to see $80 equal $01
