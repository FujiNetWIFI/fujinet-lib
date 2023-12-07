Feature: library test - network_http_post

  This tests fujinet-network atari network_http_post

  Scenario: execute _network_http_post
    Given atari-fn-nw application test setup
      And I add common src file "network_http_post.c"
      And I add file for compiling "features/test-setup/test-apps/test_ww.s"
      And I add file for compiling "features/atari/stubs/stub_network_write.s"
      And I add file for compiling "features/atari/stubs/stub_network_http_set_channel_mode_multi.s"
      And I create and load atari application
      And I write string "N:foo" as ascii to memory address $a012
      And I write word at t_w1 with hex $a012
      And I write string "data" as ascii to memory address $a123
      And I write word at t_w2 with hex $a123
      And I write word at t_fn with address _network_http_post
      And I write memory at t_m_return with $00
      And I write memory at t_m_return+1 with $96
      And I write memory at t_return with $00

     When I execute the procedure at _init for no more than 300 instructions

    # return values
    Then I expect register A equal $96
     And I expect register X equal $00

     # check parameters set correctly for network_http_set_channel_mode for both calls to it
     And I expect to see t_m_ds_l equal lo($a012)
     And I expect to see t_m_ds_h equal hi($a012)
     And I expect to see t_m_ds_l+1 equal lo($a012)
     And I expect to see t_m_ds_h+1 equal hi($a012)
     # HTTP_CHAN_MODE_POST_SET_DATA
     And I expect to see t_m_mode equal $04
     # HTTP_CHAN_MODE_BODY
     And I expect to see t_m_mode+1 equal $00

     # check params for network_write
     And I expect to see t_ds equal lo($a012)
     And I expect to see t_ds+1 equal hi($a012)
     And I expect to see t_buf equal lo($a123)
     And I expect to see t_buf+1 equal hi($a123)
     # length of "data"
     And I expect to see t_len equal lo($04)
     And I expect to see t_len+1 equal hi($04)
