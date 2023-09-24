Feature: library test - network_unit

  This tests fujinet-network network_unit

  Scenario Outline: execute _network_unit
    Given atari-fn-nw application test setup
      And I add atari src file "network_unit.s"
      And I add file for compiling "features/atari/test-apps/test_w.s"
      And I create and load application
      And I write string "<device_spec>" as ascii to memory address $a000
      And I write word at t_w1 with hex $a000
      And I write word at t_fn with address _network_unit
     When I execute the procedure at _init for no more than 30 instructions

    Then I expect register A equal <A>
     And I expect register X equal <X>

    Examples:
        | device_spec | A | X |
        | foo         | 1 | 0 |
        | n:foo       | 1 | 0 |
        | N:foo       | 1 | 0 |
        | n1:foo      | 1 | 0 |
        | n2:foo      | 2 | 0 |
        | n8:foo      | 8 | 0 |
