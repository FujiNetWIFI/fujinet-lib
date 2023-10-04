Feature: library test - atari network_status and io_status

  This tests fujinet-network atari network_status

  Scenario Outline: execute _network_status with a device spec
    Given atari-fn-nw application test setup
      And I add common atari-io files
      And I add atari src file "network_status.s"
      And I add common src file "network_unit.s"
      And I add file for compiling "features/test-setup/test-apps/test_wwww.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      And I write string "<devspec>" as ascii to memory address $a000
      # devicespec
      And I write word at t_w1 with hex $a000
      # bw
      And I write word at t_w2 with hex $a100
      # connection
      And I write word at t_w3 with hex $a200
      # device error
      And I write word at t_w4 with hex $a300
      And I write memory at $02EA with lo(<BW>)
      And I write memory at $02EB with hi(<BW>)
      And I write memory at $02EC with <CON>
      And I write memory at $02ED with <ERR>
      And I write word at t_fn with address _network_status
     When I execute the procedure at _init for no more than 200 instructions

    Then I expect register A equal <A>
     And I expect register X equal <X>
     And I expect to see $a100 equal lo(<BW>)
     And I expect to see $a101 equal hi(<BW>)
     And I expect to see $a200 equal <CON>
     And I expect to see $a300 equal <ERR>

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
    | devspec  | unit | BW  | CON | ERR | A | X |
    | n2:foo   | 2    | 512 |  7  |  1  | 0 | 0 |
    | n3:bar   | 3    |   1 |  8  |  2  | 0 | 0 |
    | n4:baz   | 4    |   0 |  9  |  3  | 0 | 0 |

  Scenario Outline: execute _network_status with a device unit
    Given atari-fn-nw application test setup
      And I add common atari-io files
      And I add atari src file "network_status.s"
      And I add common src file "network_unit.s"
      And I add file for compiling "features/test-setup/test-apps/test_bwww.s"
      And I add file for compiling "features/atari/stubs/bus_simple.s"
      And I create and load atari application
      # devicespec
      And I write memory at t_b1 with <unit>
      # bw
      And I write word at t_w2 with hex $a100
      # connection
      And I write word at t_w3 with hex $a200
      # device error
      And I write word at t_w4 with hex $a300
      And I write memory at $02EA with lo(<BW>)
      And I write memory at $02EB with hi(<BW>)
      And I write memory at $02EC with <CON>
      And I write memory at $02ED with <ERR>
      And I write word at t_fn with address _network_status_unit
     When I execute the procedure at _init for no more than 200 instructions

    Then I expect register A equal <A>
     And I expect register X equal <X>
     And I expect to see $a100 equal lo(<BW>)
     And I expect to see $a101 equal hi(<BW>)
     And I expect to see $a200 equal <CON>
     And I expect to see $a300 equal <ERR>

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
    | unit | BW  | CON | ERR | A | X |
    | 2    | 567 |  7  |  1  | 0 | 0 |
    | 3    |   1 |  8  |  2  | 0 | 0 |
    | 4    |   0 |  9  |  3  | 0 | 0 |
