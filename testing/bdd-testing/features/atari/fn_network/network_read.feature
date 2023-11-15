Feature: library test - atari network_read

  This tests fujinet-network atari network_read

  Scenario: execute _network_read when Bytes Waiting is greater than Length Specified
    Given atari-fn-nw application test setup
      And I add common atari-nw-io files
      And I add atari src file "fn_network/network_read.s"
      And I add atari src file "fn_network/sio_read.s"
      And I add common src file "network_unit.s"
      And I add atari src file "fn_network/network_status.s"
      And I add atari src file "fn_network/io_status.s"
      And I add file for compiling "features/test-setup/test-apps/test_www.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $a012
      And I write word at t_w1 with hex $a012
      And I write word at t_w2 with hex $b123
      And I write word at t_w3 with hex $1234
      And I write word at t_fn with address _network_read
      And I write word at DVSTAT with hex $2000
      # show X is changed by giving it an initial value
      And I set register X to $ff
     When I execute the procedure at _init for no more than 400 instructions

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

  Scenario: execute _network_read when Bytes Waiting is less than Length Specified uses only BW count
    Given atari-fn-nw application test setup
      And I add common atari-nw-io files
      And I add atari src file "fn_network/network_read.s"
      And I add atari src file "fn_network/sio_read.s"
      And I add common src file "network_unit.s"
      And I add atari src file "fn_network/network_status.s"
      And I add atari src file "fn_network/io_status.s"
      And I add file for compiling "features/test-setup/test-apps/test_www.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $a012
      And I write word at t_w1 with hex $a012
      And I write word at t_w2 with hex $b123
      And I write word at t_w3 with hex $1234
      And I write word at t_fn with address _network_read
      # less bytes available
      And I write word at DVSTAT with hex $0345
      # show X is changed by giving it an initial value
      And I set register X to $ff
     When I execute the procedure at _init for no more than 400 instructions

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
     And I expect to see DBYTLO equal $45
     And I expect to see DBYTHI equal $03
     And I expect to see DAUX1 equal $45
     And I expect to see DAUX2 equal $03

    # check BUS was called
    Then I expect to see $80 equal $01

  Scenario: execute _network_read when 0 Length Specified 
    Given atari-fn-nw application test setup
      And I add common atari-nw-io files
      And I add atari src file "fn_network/network_read.s"
      And I add atari src file "fn_network/sio_read.s"
      And I add common src file "network_unit.s"
      And I add atari src file "fn_network/network_status.s"
      And I add atari src file "fn_network/io_status.s"
      And I add file for compiling "features/test-setup/test-apps/test_www.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $a012
      And I write word at t_w1 with hex $a012
      And I write word at t_w2 with hex $b123
      And I write word at t_w3 with hex $0000
      And I write word at t_fn with address _network_read
      And I write memory at $80 with $ff
      And I write word at DVSTAT with hex $1234
      # show X is changed by giving it an initial value
     When I execute the procedure at _init for no more than 150 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0

    # check BUS was not called
    Then I expect to see $80 equal $ff
