Feature: library test - apple2 network_ioctl

  This tests fujinet-network apple2 network_ioctl

  Scenario: execute apple2 _network_ioctl takes len via aux params and sets payload from buffer then calls sp_control
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_ioctl.s"
      And I add file for compiling "features/apple2/invokers/test_network_ioctl1.c"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write memory at _t_cmd with $58
      And I write memory at _t_aux1 with $5
      And I write memory at _t_aux2 with $0
      And I write word at _t_buffer with hex $9000
      And I write string "1234567890" as ascii to memory address $9000
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 2500 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see _fn_device_error equal 0

    When I hex+ dump ascii between _sp_payload and _sp_payload+10
     # We only requested 5 bytes to copy in aux1/2, so should only see count of 5, and string "12345"
    Then property "test.BDD6502.lastHexDump" must contain string ": 05 00 31 32 33 34 35 00  00 00 :"
     # Validate sp_cmdlist
     And I expect to see _sp_cmdlist equal SP_STATUS_PARAM_COUNT
     And I expect memory _sp_cmdlist+1 to equal memory _sp_network
     And I expect to see _sp_cmdlist+2 equal lo(_sp_payload)
     And I expect to see _sp_cmdlist+3 equal hi(_sp_payload)
     And I expect memory _sp_cmdlist+4 to equal memory _t_cmd

  Scenario: execute apple2 _network_ioctl takes len via aux params only clears payload when NULL buffer is set
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_ioctl.s"
      And I add file for compiling "features/apple2/invokers/test_network_ioctl1.c"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write memory at _t_cmd with $58
      And I write memory at _t_aux1 with $5
      And I write memory at _t_aux2 with $0
      And I write word at _t_buffer with hex $0000
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 2300 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see _fn_device_error equal 0

    When I hex+ dump ascii between _sp_payload and _sp_payload+10
    Then property "test.BDD6502.lastHexDump" must contain string ": 05 00 00 00 00 00 00 00  00 00 :"
     # Validate sp_cmdlist
     And I expect to see _sp_cmdlist equal SP_STATUS_PARAM_COUNT
     And I expect memory _sp_cmdlist+1 to equal memory _sp_network
     And I expect to see _sp_cmdlist+2 equal lo(_sp_payload)
     And I expect to see _sp_cmdlist+3 equal hi(_sp_payload)
     And I expect memory _sp_cmdlist+4 to equal memory _t_cmd

  Scenario: execute apple2 _network_ioctl returns error when sp_network is not set
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_ioctl.s"
      And I add file for compiling "features/apple2/invokers/test_network_ioctl1.c"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 0
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 190 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal 0

  Scenario: execute apple2 _network_ioctl returns error when all args aren't set
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_ioctl.s"
      And I add file for compiling "features/apple2/invokers/test_network_ioctl2.c"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 120 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal 0
