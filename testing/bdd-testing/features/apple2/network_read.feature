Feature: library test - apple2 network_read

  This tests fujinet-network apple2 network_read

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_read with no network unit returns bad cmd and does not call sp_read
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_read.s"
      And I add apple2 src file "sp_read.s"
      And I add file for compiling "features/apple2/invokers/test_network_read.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 0
     When I execute the procedure at _init for no more than 200 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_BAD_CMD
     And I expect to see t_cb_executed equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_read with 0 len bytes returns bad cmd and does not call sp_read
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_read.s"
      And I add apple2 src file "sp_read.s"
      And I add file for compiling "features/apple2/invokers/test_network_read.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write word at t_buffer with hex $b000
      And I write word at t_len with hex $0000
      And I write memory at _sp_network with 1
     When I execute the procedure at _init for no more than 200 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_BAD_CMD
     And I expect to see t_cb_executed equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_read for under 512 bytes with no errors copies data to buffer
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_read.s"
      And I add apple2 src file "sp_read.s"
      And I add file for compiling "features/apple2/invokers/test_network_read.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write word at t_buffer with hex $b000
      And I write word at t_len with hex $000A
      And I write memory at _sp_network with 1
      # The value to return to the read call, 20 chars, but only 10 will be read
      And I write string "12345678901234567890" as ascii to memory address _sp_payload
     When I execute the procedure at _init for no more than 500 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 1
     And I expect to see _fn_device_error equal 0

    # check the data was copied to buffer, should only be 10 bytes
    When I hex+ dump ascii between $b000 and $b010
    Then property "test.BDD6502.lastHexDump" must contain string "b000: 31 32 33 34 35 36 37 38  39 30 00 00 00 00 00 00 :"
    Then property "test.BDD6502.lastHexDump" must contain string "1234567890"

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_read for with an error it returns library version of that error code
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_read.s"
      And I add apple2 src file "sp_read.s"
      And I add file for compiling "features/apple2/invokers/test_network_read.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write word at t_buffer with hex $b000
      And I write word at t_len with hex $000A
      And I write memory at _sp_network with 1
      # simulate SP_ERR_BUS_ERR error when read called
      And I write memory at t_return_code with SP_ERR_BUS_ERR
      And I write string "12345678901234567890" as ascii to memory address _sp_payload
     When I execute the procedure at _init for no more than 500 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 1
     And I expect to see _fn_device_error equal SP_ERR_BUS_ERR

    # check the data was not copied to buffer
    When I hex+ dump ascii between $b000 and $b010
    Then property "test.BDD6502.lastHexDump" must contain string "b000: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 :"

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_read for over 512 bytes with no errors copies data to buffer
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_read.s"
      And I add apple2 src file "sp_read.s"
      And I add file for compiling "features/apple2/invokers/test_network_read_long.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write word at t_buffer with hex $b000
      And I write word at t_len with hex $0201
      And I write memory at _sp_network with 1
      And I disable trace
      # sp_payload will be set by the test runner as there's 513 bytes to copy
     When I execute the procedure at _init for no more than 2550 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 2
     And I expect to see _fn_device_error equal 0

    # check the data was copied to buffer, first byte should be 96 followed by zeros
    When I hex+ dump ascii between $b000 and $b004
    Then property "test.BDD6502.lastHexDump" must contain string "b000: 96 00 00 00 :"
    # last bytes should be 69 on last byte at b200
    When I hex+ dump ascii between $b1fe and $b202
    Then property "test.BDD6502.lastHexDump" must contain string "b1fe: 00 00 69 00 :"
