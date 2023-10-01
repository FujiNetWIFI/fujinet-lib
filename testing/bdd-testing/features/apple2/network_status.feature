Feature: library test - apple2 network_status

  This tests fujinet-network apple2 network_status

  Scenario: execute apple2 _network_status with no network unit returns bad cmd and does not call sp_status
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_status.s"
      And I add file for compiling "features/apple2/invokers/test_network_status.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 0
     When I execute the procedure at _init for no more than 200 instructions

    Then I expect register A equal 2
     And I expect register X equal 0
     And I expect to see _fn_device_error equal $11
     And I expect to see t_cb_executed equal 0

  Scenario: execute apple2 _network_status with no error and all parameters set
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_status.s"
      And I add file for compiling "features/apple2/invokers/test_network_status.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write word at t_bw with hex $b000
      And I write word at t_c with hex $b002
      And I write word at t_err with hex $b003
      And I write memory at _sp_network with 1
      # bytes waiting in sp_payload/sp_payload+1
      And I write word at _sp_payload with hex $1234
      # connection in sp_payload+2
      And I write memory at _sp_payload+2 with 2
      # err in sp_payload+3
      And I write memory at _sp_payload+3 with 3
      # return no error when status called
      And I write memory at t_return_code with 0
     When I execute the procedure at _init for no more than 350 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 1
     And I expect to see _fn_device_error equal 0
     # check the values were returned to memory locations specified
     And I expect to see $b000 equal $34
     And I expect to see $b001 equal $12
     And I expect to see $b002 equal 2
     And I expect to see $b003 equal 3
