Feature: library test - network_ioctl

  This tests fujinet-network network_ioctl
  See the associated test file which calls with:
    return network_ioctl('X', 1, 2, my_devicespec, my_dstats, my_dbuf, my_dbyt);

  Scenario: execute _network_ioctl
    Given atari-fn-nw application test setup
      And I add common atari-io files
      And I add atari src file "network_ioctl.s"
      And I add atari src file "network_unit.s"
      And I add atari src file "network_status.s"
      And I add atari src file "io_status.s"
      And I add file for compiling "features/atari/test-apps/test_network_ioctl.c"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I set register X to $ff
      And I create and load application
     When I execute the procedure at _init for no more than 220 instructions

    Then I expect to see DDEVIC equal $71
     And I expect to see DUNIT equal 3
     And I expect to see DCOMND equal $58
     And I expect to see DSTATS equal $40
     And I expect to see DBUFLO equal $bc
     And I expect to see DBUFHI equal $4a
     And I expect to see DTIMLO equal $0f
     And I expect to see DBYTLO equal $34
     And I expect to see DBYTHI equal $12
     And I expect to see DAUX1 equal 1
     And I expect to see DAUX2 equal 2

    # DSTATS still in A, X forced to 0
    Then I expect register A equal $40
     And I expect register X equal 0
