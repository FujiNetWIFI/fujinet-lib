Feature: library test - network_status and io_status

  This tests fujinet-network network_status

  Scenario Outline: execute _network_status
    Given atari-fn-nw application test setup
      And I add common atari-io files
      And I add atari src file "network_status.s"
      And I add common src file "network_unit.s"
      And I add file for compiling "features/test-setup/test-apps/test_w.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "<devspec>" as ascii to memory address $a000
      And I write word at t_w1 with hex $a000
      And I write memory at $02ED with <ERR_RET>
      And I write word at t_fn with address _network_status
     When I execute the procedure at _init for no more than 100 instructions

    Then I expect register A equal <A>
     And I expect register X equal <X>

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $71
     And I expect to see DUNIT equal <unit>
     And I expect to see DCOMND equal $53
     And I expect to see DSTATS equal $40
     And I expect to see DBUFLO equal $ea
     And I expect to see DBUFHI equal $02
     And I expect to see DTIMLO equal $0f
     And I expect to see DBYTLO equal 4
     And I expect to see DBYTHI equal 0
     And I expect to see DAUX1 equal 0
     And I expect to see DAUX2 equal 0

    # check BUS was called
    Then I expect to see $80 equal $01

    Examples:
        | devspec  | unit | ERR_RET | A | X |
        | n2:foo   | 2    | 1       | 1 | 0 |
        | n3:bar   | 3    | 2       | 2 | 0 |
        | n4:baz   | 4    | 3       | 3 | 0 |

  Scenario Outline: execute _network_status_unit
    Given atari-fn-nw application test setup
      And I add common atari-io files
      And I add atari src file "network_status.s"
      And I add common src file "network_unit.s"
      And I add file for compiling "features/test-setup/test-apps/test_b.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write memory at t_b1 with <unit>
      And I write memory at $02ED with <ERR_RET>
      And I write word at t_fn with address _network_status_unit
     When I execute the procedure at _init for no more than 90 instructions

    Then I expect register A equal <A>
     And I expect register X equal <X>

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $71
     And I expect to see DUNIT equal <unit>
     And I expect to see DCOMND equal $53
     And I expect to see DSTATS equal $40
     And I expect to see DBUFLO equal $ea
     And I expect to see DBUFHI equal $02
     And I expect to see DTIMLO equal $0f
     And I expect to see DBYTLO equal 4
     And I expect to see DBYTHI equal 0
     And I expect to see DAUX1 equal 0
     And I expect to see DAUX2 equal 0

    # check BUS was called
    Then I expect to see $80 equal $01

    Examples:
        | unit | ERR_RET | A | X |
        | 2    | 1       | 1 | 0 |
        | 3    | 2       | 2 | 0 |
        | 4    | 3       | 3 | 0 |

  Scenario: execute _io_status with extended data
    Given atari-fn-nw application test setup
      And I add common atari-io files
      And I add atari src file "io_status.s"
      And I add atari src file "network_status.s"
      And I add common src file "network_unit.s"
      And I add file for compiling "features/test-setup/test-apps/test_b.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write memory at t_b1 with 1
      # Set DSTATS to 144 (extended)
      And I write memory at $1003 with 144
      And I write memory at $02ED with 10
      And I write word at t_fn with address _io_status
     When I execute the procedure at _init for no more than 90 instructions

    Then I expect register A equal 10
     And I expect register X equal 0

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $71
     And I expect to see DUNIT equal 1
     And I expect to see DCOMND equal $53
     And I expect to see DSTATS equal $40
     And I expect to see DBUFLO equal $ea
     And I expect to see DBUFHI equal $02
     And I expect to see DTIMLO equal $0f
     And I expect to see DBYTLO equal 4
     And I expect to see DBYTHI equal 0
     And I expect to see DAUX1 equal 0
     And I expect to see DAUX2 equal 0

    # check BUS was called
    Then I expect to see $80 equal $01

  Scenario: execute _io_status with no extended data
    Given atari-fn-nw application test setup
      And I add common atari-io files
      And I add atari src file "io_status.s"
      And I add atari src file "network_status.s"
      And I add common src file "network_unit.s"
      And I add file for compiling "features/test-setup/test-apps/test_b.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write memory at DDEVIC with $ff
      And I write memory at DSTATS with 2
      And I write memory at t_b1 with 1
      And I write word at t_fn with address _io_status
     When I execute the procedure at _init for no more than 30 instructions

    # check the error code is same as DSTATS
    Then I expect register A equal 2
     And I expect register X equal 0

    # check the DCB is not changed
    Then I expect to see DDEVIC equal $ff

    # check BUS was not called
    Then I expect to see $80 equal $0
