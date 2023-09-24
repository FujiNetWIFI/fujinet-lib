Feature: MACRO tests - sbw

  This tests sbw "subtract word" macro.
  The instruction count is exact to ensure the more efficient methods are used

  Scenario: sbw subtracts words
    Given atari-fn-nw simple test setup
      And I add file for compiling "features/macro_tests/test_sbw.s"
      And I create and load simple application

     ########################################################
     # WORD, #WORD
     When I execute the procedure at test_sbw_word_immw for no more than 12 instructions     
     Then I expect to see t_t1 equal lo($3210)
      And I expect to see t_t1+1 equal hi($3210)

     ########################################################
     # WORD, WORD
     When I execute the procedure at test_sbw_word_word for no more than 12 instructions
     Then I expect to see t_t2 equal lo($3322)
      And I expect to see t_t2+1 equal hi($3322)

     ########################################################
     # WORD, #WORD, WORD
     When I execute the procedure at test_sbw_word_immw_word for no more than 8 instructions
     Then I expect to see t_t3 equal lo($20ff)
      And I expect to see t_t3+1 equal hi($20ff)

     ########################################################
     # WORD, WORD, WORD
     When I execute the procedure at test_sbw_word_word_word for no more than 8 instructions
     Then I expect to see t_t4 equal lo($3322)
      And I expect to see t_t4+1 equal hi($3322)

     ########################################################
     # WORD, #BYTE - NO CARRY
     When I execute the procedure at test_sbw_word_imm_no_c for no more than 10 instructions
     Then I expect to see t_t5 equal lo($4320)
      And I expect to see t_t5+1 equal hi($4320)

     ########################################################
     # WORD, #BYTE - WITH CARRY
     When I execute the procedure at test_sbw_word_imm_c for no more than 11 instructions
     Then I expect to see t_t6 equal lo($4254)
      And I expect to see t_t6+1 equal hi($4254)

     ########################################################
     # WORD, #BYTE, WORD - NO CARRY
     When I execute the procedure at test_sbw_word_imm_word_no_c for no more than 8 instructions
     Then I expect to see t_t7 equal lo($4320)
      And I expect to see t_t7+1 equal hi($4320)

     ########################################################
     # WORD, #BYTE, WORD - WITH CARRY
     When I execute the procedure at test_sbw_word_imm_word_c for no more than 9 instructions
     Then I expect to see t_t8 equal lo($4254)
      And I expect to see t_t8+1 equal hi($4254)
