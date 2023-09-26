Feature: MACRO tests - adw

  This tests adw "add word" macro.
  The instruction count is exact to ensure the more efficient methods are used

  Scenario: adw adds words
    Given atari-fn-nw simple test setup
      And I add file for compiling "features/macro_tests/test_adw.s"
      And I create and load simple atari application

     ########################################################
     # WORD, #WORD
     When I execute the procedure at test_adw_word_immw for no more than 12 instructions     
     Then I expect to see t_t1 equal lo($2345)
      And I expect to see t_t1+1 equal hi($2345)

     ########################################################
     # WORD, WORD
     When I execute the procedure at test_adw_word_word for no more than 12 instructions
     Then I expect to see t_t2 equal lo($b247)
      And I expect to see t_t2+1 equal hi($b247)

     ########################################################
     # WORD, #WORD, WORD
     When I execute the procedure at test_adw_word_immw_word for no more than 8 instructions
     Then I expect to see t_t3 equal lo($3456)
      And I expect to see t_t3+1 equal hi($3456)

     ########################################################
     # WORD, WORD, WORD
     When I execute the procedure at test_adw_word_word_word for no more than 8 instructions
     Then I expect to see t_t4 equal lo($b124)
      And I expect to see t_t4+1 equal hi($b124)

     ########################################################
     # WORD, #BYTE - NO CARRY
     When I execute the procedure at test_adw_word_imm_no_c for no more than 10 instructions
     Then I expect to see t_t5 equal lo($1235)
      And I expect to see t_t5+1 equal hi($1235)

     ########################################################
     # WORD, #BYTE - WITH CARRY
     When I execute the procedure at test_adw_word_imm_c for no more than 11 instructions
     Then I expect to see t_t6 equal lo($1301)
      And I expect to see t_t6+1 equal hi($1301)

     ########################################################
     # WORD, #BYTE, WORD - NO CARRY
     When I execute the procedure at test_adw_word_imm_word_no_c for no more than 8 instructions
     Then I expect to see t_t7 equal lo($1235)
      And I expect to see t_t7+1 equal hi($1235)

     ########################################################
     # WORD, #BYTE, WORD - WITH CARRY
     When I execute the procedure at test_adw_word_imm_word_c for no more than 9 instructions
     Then I expect to see t_t8 equal lo($1301)
      And I expect to see t_t8+1 equal hi($1301)
