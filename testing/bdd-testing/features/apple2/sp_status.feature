Feature: library test - apple2 sp_status

  This tests fujinet-network apple2 sp_status

  Scenario: execute sp.c
    Given apple2-fn-nw application test setup
      # And I add common apple2-io files
      And I add apple2 src file "sp.c"
      And I add file for compiling "features/test-setup/test-apps/test_0.s"
      And I create and load apple-single application
      And I write word at t_fn with address _sp_status
     When I execute the procedure at _init for no more than 500 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
