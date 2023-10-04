Feature: library test - apple2 network_json_parse

  This tests fujinet-network apple2 network_json_parse


  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_parse returns ok (happy path)
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_json_parse.s"
      And I add file for compiling "features/apple2/invokers/test_network_json_parse.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
     When I execute the procedure at _init for no more than 2000 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 2

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_parse with no network unit returns bad cmd and does not call sp functions
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_json_parse.s"
      And I add file for compiling "features/apple2/invokers/test_network_json_parse.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 0
     When I execute the procedure at _init for no more than 200 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_BAD_UNIT
     And I expect to see t_cb_executed equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_parse with error calling control for json channel mode passes it to caller
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_json_parse.s"
      And I add file for compiling "features/apple2/invokers/test_network_json_parse.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write memory at t_r1_error with SP_ERR_IO_ERROR
     When I execute the procedure at _init for no more than 2000 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_IO_ERROR
     And I expect to see t_cb_executed equal 1
     And I expect to see t_r1_cmd equal SP_CMD_CONTROL
     # in the case of a CONTROL command, a is same as cmd
     And I expect to see t_cb_a+0 equal SP_CMD_CONTROL
     # json channel mode
     And I expect to see t_cb_codes+0 equal $FC
     And I expect to see t_r1_unit equal 1
     And I expect to see t_r1_payload equal 1
     And I expect to see t_r1_payload+1 equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_parse with error doing PARSE control
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_json_parse.s"
      And I add file for compiling "features/apple2/invokers/test_network_json_parse.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write memory at t_r2_error with SP_ERR_IO_ERROR
     When I execute the procedure at _init for no more than 2000 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_IO_ERROR
     And I expect to see t_cb_executed equal 2
     And I expect to see t_r2_cmd equal SP_CMD_CONTROL
     And I expect to see t_cb_a+1 equal SP_CMD_CONTROL
     # 'P'
     And I expect to see t_cb_codes+1 equal 80
     And I expect to see t_r2_unit equal 1
