Feature: library test - apple2 network_json_query

  This tests fujinet-network apple2 network_json_query

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_query returns query results (happy path)
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_query.s"
      And I add apple2 src file "fn_fuji/sp_read.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_query.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write string "/bar" as ascii to memory address $9100
      And I write word at t_query with hex $9100
      And I write string "1234567890" as ascii to memory address t_read_string
      # ensure there target location has some data so we can prove we overwrote it
      And I write string "XXXXXXXXXXXXX" as ascii to memory address $9200
      And I write word at t_s with hex $9200
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 3250 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 3

     And I expect to see t_r1_cmd equal SP_CMD_CONTROL
     # in the case of a CONTROL command, a is same as cmd
     And I expect to see t_cb_a+0 equal SP_CMD_CONTROL
     # 'Q'
     And I expect to see t_cb_codes+0 equal 81
     And I expect to see t_r1_unit equal 1
     # Payload should just have 05 (length of query plus nul) at position 0
     And I expect to see t_r1_payload equal 5
     And I expect to see t_r1_payload+1 equal 0
     # should see the query string "/bar" at locations payload[2,5]
     When I hex+ dump ascii between t_r1_payload+2 and t_r1_payload+6
     Then property "test.BDD6502.lastHexDump" must contain string "/bar"

     And I expect to see t_r2_cmd equal SP_CMD_STATUS
     # in case of the STATUS, a is the code instead, here "S" for status, this is just testing the emulator was called correctly
     And I expect to see t_cb_a+1 equal 83
     And I expect to see t_cb_codes+1 equal 83
     And I expect to see t_r2_unit equal 1

     And I expect to see t_r3_cmd equal SP_CMD_READ
     And I expect to see t_cb_a+2 equal SP_CMD_READ
     # cmdlist[4] is codes value, for sp_read, it's the lo byte of length of the read instead of ctrlcode/statcode.
     # for the test, it's 10 by default
     And I expect to see t_cb_codes+2 equal 10
     And I expect to see t_r3_unit equal 1
     When I hex+ dump ascii between t_r1_payload+2 and t_r1_payload+6
     Then property "test.BDD6502.lastHexDump" must contain string "/bar"

     # copied the "read" string with nul to destination, not overwriting final X ($58)
     When I hex+ dump ascii between $9200 and $920C
     Then property "test.BDD6502.lastHexDump" must contain string "9200: 31 32 33 34 35 36 37 38  39 30 00 58 :"

     # sp_payload was zeroed out, so only bytes in there are the ones we have from the query, this is the state BEFORE doing read
     When I hex+ dump ascii between t_r2_payload and t_r2_payload+8
     Then property "test.BDD6502.lastHexDump" must contain string ": 05 00 2f 62 61 72 00 00 :"
     Then property "test.BDD6502.lastHexDump" must contain string "/bar"

     # final Payload after sp_read contains length, then string, then 0s
     When I hex+ dump ascii between _sp_payload and _sp_payload+16
     Then property "test.BDD6502.lastHexDump" must contain string ": 31 32 33 34 35 36 37 38  39 30 00 00 00 00 00 00 :"
     Then property "test.BDD6502.lastHexDump" must contain string "1234567890"


  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_query with no network unit returns bad cmd and does not call sp functions
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_query.s"
      And I add apple2 src file "fn_fuji/sp_read.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_query.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 0
      And ignore address _bzero to _bzero+93 for trace
     When I execute the procedure at _init for no more than 2070 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_BAD_UNIT
     And I expect to see t_cb_executed equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_query with query error passes it to caller and sets empty string
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_query.s"
      And I add apple2 src file "fn_fuji/sp_read.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_query.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write string "/bar" as ascii to memory address $9100
      And I write word at t_query with hex $9100
      # ensure there target location has some data so we can prove we overwrote it
      And I write string "XXXX" as ascii to memory address $9200
      And I write word at t_s with hex $9200
      And I write memory at t_r1_error with SP_ERR_IO_ERROR
      And ignore address _bzero to _bzero+93 for trace
     When I execute the procedure at _init for no more than 2500 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_IO_ERROR
     And I expect to see t_cb_executed equal 1
     And I expect to see t_r1_cmd equal SP_CMD_CONTROL
     # in the case of a CONTROL command, a is same as cmd
     And I expect to see t_cb_a+0 equal SP_CMD_CONTROL
     # 'Q'
     And I expect to see t_cb_codes+0 equal 81
     And I expect to see t_r1_unit equal 1
     # Payload should just have 05 (length of query + nul) at position 0
     And I expect to see t_r1_payload equal 5
     And I expect to see t_r1_payload+1 equal 0
     # should see the query string "/bar" at locations payload[2,5]
     When I hex+ dump ascii between t_r1_payload+2 and t_r1_payload+6
     Then property "test.BDD6502.lastHexDump" must contain string "/bar"
     # show there is a zero (nul string) in target string address, but only first byte, next 2 bytes are "X" still ($58)
     When I hex+ dump ascii between $9200 and $9203
     Then property "test.BDD6502.lastHexDump" must contain string "9200: 00 58 58 :"

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_query with query that returns an error from the status getting length
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_query.s"
      And I add apple2 src file "fn_fuji/sp_read.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_query.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write string "/bar" as ascii to memory address $9100
      And I write word at t_query with hex $9100
      # ensure there target location has some data so we can prove we overwrote it
      And I write string "XXXX" as ascii to memory address $9200
      And I write word at t_s with hex $9200
      And I write memory at t_r2_error with SP_ERR_IO_ERROR
      And ignore address _bzero to _bzero+93 for trace
     When I execute the procedure at _init for no more than 2800 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see _fn_device_error equal SP_ERR_IO_ERROR
     And I expect to see t_cb_executed equal 2
     And I expect to see t_r2_cmd equal SP_CMD_STATUS
     # in case of the STATUS, a is the code instead, here "S" for status, this is just testing the emulator was called correctly
     And I expect to see t_cb_a+1 equal 83
     And I expect to see t_cb_codes+1 equal 83
     And I expect to see t_r2_unit equal 1

     # show there is a zero (nul string) in target string address, but only first byte, next 2 bytes are "X" still ($58)
     When I hex+ dump ascii between $9200 and $9203
     Then property "test.BDD6502.lastHexDump" must contain string "9200: 00 58 58 :"

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_query that returns 0 byte length we write empty string to target
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_query.s"
      And I add apple2 src file "fn_fuji/sp_read.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_query.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write string "/bar" as ascii to memory address $9100
      And I write word at t_query with hex $9100
      # ensure there target location has some data so we can prove we overwrote it
      And I write string "XXXX" as ascii to memory address $9200
      And I write word at t_s with hex $9200
      And I write memory at t_status_len_ret with 0
      And ignore address _bzero to _bzero+93 for trace
     When I execute the procedure at _init for no more than 2800 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 2
     And I expect to see t_r2_cmd equal SP_CMD_STATUS
     And I expect to see t_cb_a+1 equal 83
     And I expect to see t_cb_codes+1 equal 83
     And I expect to see t_r2_unit equal 1

     # show there is a zero (nul string) in target string address, but only first byte, next 2 bytes are "X" still ($58)
     When I hex+ dump ascii between $9200 and $9203
     Then property "test.BDD6502.lastHexDump" must contain string "9200: 00 58 58 :"

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute apple2 _network_json_query and read returns an error we pass that back and write empty string
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "fn_network/network_json_query.s"
      And I add apple2 src file "fn_fuji/sp_read.s"
      And I add file for compiling "features/apple2/fn_network/invokers/test_network_json_query.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at _sp_network with 1
      And I write string "n1:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write string "/bar" as ascii to memory address $9100
      And I write word at t_query with hex $9100
      # ensure there target location has some data so we can prove we overwrote it
      And I write string "XXXX" as ascii to memory address $9200
      And I write word at t_s with hex $9200
      And I write memory at t_r3_error with SP_ERR_IO_ERROR
      And ignore address _bzero to _bzero+93 for trace
     When I execute the procedure at _init for no more than 3150 instructions

    Then I expect register A equal FN_ERR_IO_ERROR
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 3
     And I expect to see t_r3_cmd equal SP_CMD_READ
     And I expect to see t_cb_a+2 equal SP_CMD_READ
     # cmdlist[4] is codes value, for sp_read, it's the lo byte of length of the read instead of ctrlcode/statcode.
     # for the test, it's 10 by default
     And I expect to see t_cb_codes+2 equal 10
     And I expect to see t_r3_unit equal 1

     # show there is a zero (nul string) in target string address, but only first byte, next 2 bytes are "X" still ($58)
     When I hex+ dump ascii between $9200 and $9203
     Then property "test.BDD6502.lastHexDump" must contain string "9200: 00 58 58 :"

