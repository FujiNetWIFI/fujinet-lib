Feature: IO library test - fn_fuji_hash_output

  This tests FN-FUJI fn_fuji_hash_output

  Scenario: execute _fuji_hash_output
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fuji_hash_output.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_bww.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      And I write memory at t_b1 with $02
      And I write word at t_w2 with hex 1234
      And I write word at t_w3 with hex abcd
      And I write word at t_fn with address _fuji_hash_output
     When I execute the procedure at _init for no more than 130 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DCOMND equal $c5
     And I expect to see DSTATS equal $40
     And I expect to see DBUFLO equal $34
     And I expect to see DBUFHI equal $12
     And I expect to see DTIMLO equal 3
     And I expect to see DBYTLO equal $cd
     And I expect to see DBYTHI equal $ab
     And I expect to see DAUX1 equal 2
     And I expect to see DAUX2 equal 0

    # check BUS was called
    Then I expect to see $80 equal $01
