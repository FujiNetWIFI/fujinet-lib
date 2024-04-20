Feature: IO library test - fn_fuji_base64_decode_length

  This tests FN-FUJI fn_fuji_base64_decode_length

  Scenario: execute _fuji_base64_decode_length
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fuji_base64_xxcode_length.s"
      And I add atari src file "fn_fuji/fuji_crypto_common.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_w.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      And I write word at t_w1 with hex a012
      And I write word at t_fn with address _fuji_base64_decode_length
     When I execute the procedure at _init for no more than 85 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DCOMND equal $ca
     And I expect to see DSTATS equal $40
     And I expect to see DBUFLO equal $12
     And I expect to see DBUFHI equal $a0
     And I expect to see DTIMLO equal 3
     And I expect to see DBYTLO equal 4
     And I expect to see DBYTHI equal 0
     And I expect to see DAUX1 equal 0
     And I expect to see DAUX2 equal 0

    # check BUS was called
    Then I expect to see $80 equal $01
