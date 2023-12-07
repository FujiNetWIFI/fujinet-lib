Feature: library test - network_http_put

  This tests fujinet-network atari network_http_put

  Scenario: execute _network_http_put
    Given atari-fn-nw application test setup
      And I add common src file "network_http_put.c"
      And I add file for compiling "features/test-setup/test-apps/test_ww.s"
      And I add file for compiling "features/atari/stubs/stub_network_post.s"
      And I create and load atari application
      And I write string "N:foo" as ascii to memory address $a012
      And I write word at t_w1 with hex $a012
      And I write string "data" as ascii to memory address $a123
      And I write word at t_w2 with hex $a123
      And I write word at t_fn with address _network_http_put
      And I write memory at t_return with $96

     When I execute the procedure at _init for no more than 120 instructions

    # return values
    Then I expect register A equal $96
     And I expect register X equal $00

     # check parameters set correctly
     And I expect to see t_ds equal lo($a012)
     And I expect to see t_ds+1 equal hi($a012)
     And I expect to see t_data equal lo($a123)
     And I expect to see t_data+1 equal hi($a123)
