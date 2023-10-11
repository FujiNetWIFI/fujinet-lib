Feature: library test - apple2 network_ioctl

  This tests fujinet-network apple2 network_ioctl

  Scenario: execute apple2 _network_ioctl is not implemented and returns bad command
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_ioctl.s"
      And I add file for compiling "features/apple2/invokers/test_network_ioctl.c"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      # tell the test invoker to call with wrong args
      And I write memory at _t_do_bad_args with 1
      And I ignore cc65-noise
     When I execute the procedure at _init for no more than 2020 instructions

    Then I expect register A equal FN_ERR_BAD_CMD
     And I expect register X equal 0
     And I expect to see _fn_device_error equal 0
