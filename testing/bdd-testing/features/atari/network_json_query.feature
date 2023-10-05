Feature: library test - atari network_json_query

  This tests fujinet-network atari network_json_query

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute _network_json_query returns the expected data
    Given atari-fn-nw application test setup
      And I enable trace with indent
      And I add common atari-io files
      And I add atari src file "network_json_query.s"
      And I add file for compiling "features/atari/invokers/test_network_json_query.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write string "/bar" as ascii to memory address $9100
      And I write word at t_query with hex $9100
      And I write word at t_s with hex $9200
      And I write word at DVSTAT with hex $0006
      And I write string "{json}" as ascii to memory address t_read_data
      # check what gets overwritten in the target buffer
      And I write string "XXXXXXXX" as ascii to memory address $9200
      And I write memory at fn_open_mode_table with $c
     When I execute the procedure at _init for no more than 500 instructions

     # Validate every function was called with correct values.
     Then I expect to see t_ioctl_dbyt equal 0
      And I expect to see t_ioctl_dbyt+1 equal 1
      And I expect to see t_ioctl_dbuf equal 0
      And I expect to see t_ioctl_dbuf+1 equal $91
      And I expect to see t_ioctl_dstats equal $80
      And I expect to see t_ioctl_dstats+1 equal 0
      And I expect to see t_ioctl_devspec equal 0
      And I expect to see t_ioctl_devspec+1 equal $90
      And I expect to see t_ioctl_aux1 equal $c
      And I expect to see t_ioctl_aux2 equal 0
      # 'Q'
      And I expect to see t_ioctl_cmd equal 81
      And I expect to see t_network_status_devicespec equal 0
      And I expect to see t_network_status_devicespec+1 equal $90
      And I expect to see t_network_status_bw equal lo(_fn_network_bw)
      And I expect to see t_network_status_bw+1 equal hi(_fn_network_bw)
      And I expect to see t_network_status_conn equal lo(_fn_network_conn)
      And I expect to see t_network_status_conn+1 equal hi(_fn_network_conn)
      And I expect to see t_network_status_err equal lo(_fn_network_error)
      And I expect to see t_network_status_err+1 equal hi(_fn_network_error)

      And I expect to see t_network_read_devicespec equal 0
      And I expect to see t_network_read_devicespec+1 equal $90
      And I expect to see t_network_read_buf equal 0
      And I expect to see t_network_read_buf+1 equal $92
      And I expect to see t_network_read_len equal 6
      And I expect to see t_network_read_len+1 equal 0

     # check the string was copied to the destination, with a trailing 0 to terminate the string
     # The location held all XXXX chars, we show we overwrite exactly 6+nul, and the extra chars are untouched (final 58 is an X)
     When I hex+ dump ascii between $9200 and $9208
     # {json}<0>X
     Then property "test.BDD6502.lastHexDump" must contain string "9200: 7b 6a 73 6f 6e 7d 00 58 :"

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute _network_json_query handles ioctl error
    Given atari-fn-nw application test setup
      And I enable trace with indent
      And I add common atari-io files
      And I add atari src file "network_json_query.s"
      And I add file for compiling "features/atari/invokers/test_network_json_query.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write word at t_query with hex $9100
      And I write word at t_s with hex $9200
      # force an ioctl error
      And I write memory at t_is_ioctl_error with 1
      And I write word at DVSTAT with hex $0006
     When I execute the procedure at _init for no more than 300 instructions
    Then I expect register A equal 1
     And I expect register X equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute _network_json_query handles status error
    Given atari-fn-nw application test setup
      And I enable trace with indent
      And I add common atari-io files
      And I add atari src file "network_json_query.s"
      And I add file for compiling "features/atari/invokers/test_network_json_query.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write word at t_query with hex $9100
      And I write word at t_s with hex $9200
      # force a status error
      And I write memory at t_is_status_error with 1
      And I write word at DVSTAT with hex $0006
     When I execute the procedure at _init for no more than 400 instructions
    Then I expect register A equal 1
     And I expect register X equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute _network_json_query handles read error
    Given atari-fn-nw application test setup
      And I enable trace with indent
      And I add common atari-io files
      And I add atari src file "network_json_query.s"
      And I add file for compiling "features/atari/invokers/test_network_json_query.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write word at t_query with hex $9100
      And I write word at t_s with hex $9200
      And I write memory at t_is_read_error with 1
      And I write word at DVSTAT with hex $0006
     When I execute the procedure at _init for no more than 440 instructions
    Then I expect register A equal 1
     And I expect register X equal 0

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute _network_json_query handles 0 byte read and writes empty string with no error
    Given atari-fn-nw application test setup
      And I enable trace with indent
      And I add common atari-io files
      And I add atari src file "network_json_query.s"
      And I add file for compiling "features/atari/invokers/test_network_json_query.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write word at t_query with hex $9100
      And I write word at t_s with hex $9200
      And I write word at DVSTAT with hex $0000
      And I write string "XXXXXXXX" as ascii to memory address $9200
     When I execute the procedure at _init for no more than 400 instructions
    Then I expect register A equal 0
     And I expect register X equal 0
     When I hex+ dump ascii between $9200 and $9203
     # <0>X
     Then property "test.BDD6502.lastHexDump" must contain string "9200: 00 58 58 :"
