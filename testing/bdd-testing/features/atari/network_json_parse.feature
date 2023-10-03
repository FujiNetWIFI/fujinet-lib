Feature: library test - atari network_json_parse

  This tests fujinet-network atari network_json_parse

  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute _network_json_parse returns the expected data
    Given atari-fn-nw application test setup
      And I enable trace with indent
      And I add common atari-io files
      And I add atari src file "network_json_parse.s"
      And I add file for compiling "features/atari/invokers/test_network_json_parse.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write memory at t_mode with $69
     When I execute the procedure at _init for no more than 400 instructions

     # Validate every function was called with correct values.
     Then I expect to see t_ioctl_dbyt equal 0
      And I expect to see t_ioctl_dbyt+1 equal 0
      And I expect to see t_ioctl_dbuf equal 0
      And I expect to see t_ioctl_dbuf+1 equal 0
      And I expect to see t_ioctl_dstats equal 0
      And I expect to see t_ioctl_dstats+1 equal 0
      And I expect to see t_ioctl_devspec equal 0
      And I expect to see t_ioctl_devspec+1 equal $90
      And I expect to see t_ioctl_aux1 equal $69
      And I expect to see t_ioctl_aux2 equal 0
      # 'P'
      And I expect to see t_ioctl_cmd equal 80

      And I expect register A equal 0
      And I expect register X equal 0


  # -----------------------------------------------------------------------------------------------------------------
  Scenario: execute _network_json_parse handles ioctl error
    Given atari-fn-nw application test setup
      And I enable trace with indent
      And I add common atari-io files
      And I add atari src file "network_json_parse.s"
      And I add file for compiling "features/atari/invokers/test_network_json_parse.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "n9:foo" as ascii to memory address $9000
      And I write word at t_devicespec with hex $9000
      And I write memory at t_mode with $01
      # force an ioctl error
      And I write memory at t_is_ioctl_error with 1
     When I execute the procedure at _init for no more than 240 instructions
    Then I expect register A equal 1
     And I expect register X equal 0
