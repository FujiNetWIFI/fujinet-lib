Feature: library test - apple2 network_open

  This tests fujinet-network apple2 network_open

  Scenario: execute apple2 _network_open finding no network device returns io error
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_open.s"
      And I add file for compiling "features/apple2/invokers/test_network_open.s"
      And I add file for compiling "../../apple2/src/sp_find_network.s"
      And I add file for compiling "../../apple2/src/sp_open.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 0
      And I write memory at spe_should_fail_device_lookup with 1
      And I write memory at spe_num_devices with 1
     When I execute the procedure at _init for no more than 460 instructions

    # FN_IO_ERROR returned
    Then I expect register A equal 1
     And I expect register X equal 0
    # real device error is SP_ERR_IO_ERROR
     And I expect to see _fn_device_error equal $27
     And I expect to see t_cb_executed equal 0

  Scenario: execute apple2 _network_open finding no network device returns IO error
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_open.s"
      And I add file for compiling "features/apple2/invokers/test_network_open.s"
      And I add file for compiling "../../apple2/src/sp_find_network.s"
      And I add file for compiling "../../apple2/src/sp_open.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 0
      And I write memory at spe_should_fail_device_lookup with 1
      And I write memory at spe_num_devices with 1
     When I execute the procedure at _init for no more than 460 instructions

    # FN_IO_ERROR returned
    Then I expect register A equal 1
     And I expect register X equal 0
    # real device error is SP_ERR_IO_ERROR
     And I expect to see _fn_device_error equal $27
     And I expect to see t_cb_executed equal 0


  Scenario: execute apple2 _network_open sets payload data correctly when there is a valid network
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_open.s"
      And I add file for compiling "features/apple2/invokers/test_network_open.s"
      And I add file for compiling "../../apple2/src/sp_find_network.s"
      And I add file for compiling "../../apple2/src/sp_open.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write memory at t_mode with $04
      And I write memory at t_translate with $80
     When I execute the procedure at _init for no more than 1500 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 2
     And I expect to see spe_cmd equal SP_CMD_CONTROL
     # DEST is smartport NETWORK device in emulator (2)
     And I expect to see spe_dest equal 2
     # 'O' control code
     And I expect to see _sp_cmdlist+4 equal 79
     # network is unit 2, table is 0 based
     And I expect to see fn_open_mode equal 4

     # check sp_open was called correctly
     And I expect to see t_r1_cmd equal SP_CMD_OPEN
     # Network unit is 2 in smartport emulator
     And I expect to see t_r1_unit equal 2

    When I hex+ dump ascii between t_r1_payload+5 and t_r1_payload+12
    Then property "test.BDD6502.lastHexDump" must contain string "NETWORK"

     # validate payload
     #  [0] = strlen+2
     #  [1] = 0
     #  [2] = mode
     #  [3] = translation
     #  [4..4+len] = devicespec
     #  [..+1] = 0x00
     #  ... = $00
    When I hex+ dump ascii between t_r2_payload and t_r2_payload+11
    # exact bytes, and string version
    Then property "test.BDD6502.lastHexDump" must contain string ": 08 00 04 80 6e 35 3a 66  6f 6f 00 :"
    Then property "test.BDD6502.lastHexDump" must contain string "n5:foo"
