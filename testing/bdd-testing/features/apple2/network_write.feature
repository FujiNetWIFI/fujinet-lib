Feature: library test - apple2 network_write

  This tests fujinet-network apple2 network_write

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_write with no network unit returns bad cmd and does not call sp_write
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_write.s"
      And I add apple2 src file "sp_write.s"
      And I add file for compiling "features/apple2/invokers/test_network_write.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write memory at _sp_network with 0
     When I execute the procedure at _init for no more than 200 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_BAD_CMD
     And I expect to see t_cb_executed equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_write with 0 len bytes returns bad cmd and does not call sp_write
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_write.s"
      And I add apple2 src file "sp_write.s"
      And I add file for compiling "features/apple2/invokers/test_network_write.s"
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
  Scenario: execute apple2 _network_write for under 512 bytes with no errors copies data from buffer
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_write.s"
      And I add apple2 src file "sp_write.s"
      And I add file for compiling "features/apple2/invokers/test_network_write.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write word at t_buffer with hex $b000
      And I write word at t_len with hex $000A
      And I write memory at _sp_network with 1
      # setup the buffer to push values from
      And I write memory at $b000 with $01
      And I write memory at $b009 with $09
      And I write memory at $b00a with $0a
     When I execute the procedure at _init for no more than 500 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 1
     And I expect to see _fn_device_error equal 0
     And I expect to see _sp_payload equal $01
     And I expect to see _sp_payload+9 equal $09
     # only 10 bytes copied (0-9)
     And I expect to see _sp_payload+10 equal $00

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_write for with an error it returns library version of that error code
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_write.s"
      And I add apple2 src file "sp_write.s"
      And I add file for compiling "features/apple2/invokers/test_network_write.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write word at t_buffer with hex $b000
      And I write word at t_len with hex $000A
      And I write memory at _sp_network with 1
      # simulate SP_ERR_BUS_ERR error when read called
      And I write memory at t_return_code with SP_ERR_BUS_ERR
     When I execute the procedure at _init for no more than 500 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 1
     And I expect to see _fn_device_error equal SP_ERR_BUS_ERR

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_write for over 512 bytes with no errors copies data to buffer
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_write.s"
      And I add apple2 src file "sp_write.s"
      And I add file for compiling "features/apple2/invokers/test_network_write_long.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write word at t_buffer with hex $b000
      And I write word at t_len with hex $0201
      And I write memory at _sp_network with 1
      # setup the buffer to push values from
      And I write memory at $b000 with $96
      And I write memory at $b1ff with $aa
      And I write memory at $b200 with $69
     When I execute the procedure at _init for no more than 2550 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 2
     And I expect to see _fn_device_error equal 0
     And I expect to see t_r1b1 equal $96
     And I expect to see t_r1b2 equal $aa
     And I expect to see t_r2b1 equal $69
