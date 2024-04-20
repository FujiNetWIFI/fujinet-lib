Feature: library test - apple2 network_json_parse

  This tests fujinet-network apple2 network_json_parse


  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_parse returns ok (happy path)
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_parse.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_parse.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write memory at fn_open_mode with $0c
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 2570 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 2

     # json channel mode
     And I expect to see t_cb_codes+0 equal $FC
     # only one network device on apple and it's the id of the network device - TODO: check this
     And I expect to see t_r1_unit equal 2
     And I expect to see t_r1_payload equal 1
     And I expect to see t_r1_payload+1 equal 0
     And I expect to see t_r1_payload+2 equal 1

     # 'P' for parse
     And I expect to see t_cb_codes+1 equal 80
     # only one network device on apple and it's the id of the network device - TODO: check this
     And I expect to see t_r2_unit equal 2
     # Unchanged from previous call - doesn't really prove anything, these bytes aren't used
     And I expect to see t_r2_payload equal 1
     And I expect to see t_r2_payload+1 equal 0
     And I expect to see t_r2_payload+2 equal 1


  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_parse with no network unit returns bad cmd and does not call sp functions
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_parse.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_parse.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      # ensure _sp_network is reset to 0 so we can test what happens when it is
      And I write memory at t_sp_network with $00
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 1850 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_BAD_UNIT
     And I expect to see t_cb_executed equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_parse with error calling control for json channel mode passes it to caller
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_parse.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_parse.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write memory at t_r1_error with SP_ERR_IO_ERROR
      And I write memory at fn_open_mode with $0c
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 2300 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_IO_ERROR
     And I expect to see t_cb_executed equal 1
     And I expect to see t_r1_cmd equal SP_CMD_CONTROL
     # in the case of a CONTROL command, a is same as cmd
     And I expect to see t_cb_a+0 equal SP_CMD_CONTROL
     # json channel mode
     And I expect to see t_cb_codes+0 equal $FC
     # only one network device on apple and it's the id of the network device - TODO: check this
     And I expect to see t_r1_unit equal 2
     And I expect to see t_r1_payload equal 1
     And I expect to see t_r1_payload+1 equal 0
     And I expect to see t_r1_payload+2 equal 1

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_parse with error doing PARSE control
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_parse.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_parse.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write memory at t_r2_error with SP_ERR_IO_ERROR
      And I write memory at fn_open_mode with $0c
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 2600 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_IO_ERROR
     And I expect to see t_cb_executed equal 2
     And I expect to see t_r2_cmd equal SP_CMD_CONTROL
     And I expect to see t_cb_a+1 equal SP_CMD_CONTROL
     # 'P'
     And I expect to see t_cb_codes+1 equal 80
     # only one network device on apple and it's the id of the network device - TODO: check this
     And I expect to see t_r2_unit equal 2
