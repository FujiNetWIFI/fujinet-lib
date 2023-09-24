Feature: MACRO tests - mwa

  This tests mwa macro.

  Scenario: mwa moves word via the a register to targets
    Given atari-fn-nw simple test setup
      And I add file for compiling "features/macro_tests/test_mwa.s"
      And I create and load simple application

     When I execute the procedure at test_mwa for no more than 200 instructions
     
     Then I expect to see t_t1 equal lo($1234)
      And I expect to see t_t1+1 equal hi($1234)
      And I expect to see t_t2 equal lo($abcd)
      And I expect to see t_t2+1 equal hi($abcd)
      And I expect to see t_t3 equal lo(t_t1)
      And I expect to see t_t3+1 equal hi(t_t1)
