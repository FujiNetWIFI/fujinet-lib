Feature: library test - atari network_read

  This tests fujinet-network atari network_read

  Scenario: execute _network_read
    Given atari-fn-nw application test setup
      And I add common atari-io files
      And I add atari src file "network_read_write.s"
      And I add common src file "network_unit.s"
      And I add atari src file "network_status.s"
      And I add atari src file "io_status.s"
      And I add file for compiling "features/test-setup/test-apps/test_www.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $a012
      And I write word at t_w1 with hex $a012
      And I write word at t_w2 with hex $b123
      And I write word at t_w3 with hex $1234
      And I write word at t_fn with address _network_read
      # show X is changed by giving it an initial value
      And I set register X to $ff
     When I execute the procedure at _init for no more than 170 instructions

    Then I expect register A equal 0
     And I expect register X equal 0

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $71
     And I expect to see DUNIT equal 9
     And I expect to see DCOMND equal $52
     And I expect to see DSTATS equal $40
     And I expect to see DBUFLO equal $23
     And I expect to see DBUFHI equal $b1
     And I expect to see DTIMLO equal $0f
     And I expect to see DBYTLO equal $34
     And I expect to see DBYTHI equal $12
     And I expect to see DAUX1 equal $34
     And I expect to see DAUX2 equal $12

    # check BUS was called
    Then I expect to see $80 equal $01
