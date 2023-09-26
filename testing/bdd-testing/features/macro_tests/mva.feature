Feature: MACRO tests - mva

  This tests mva macro.

  Scenario: mva moves single bytes via the a register to targets
    Given atari-fn-nw simple test setup
      And I add file for compiling "features/macro_tests/test_mva.s"
      And I create and load simple atari application

     When I execute the procedure at test_mva for no more than 200 instructions
     
     Then I expect to see $80 equal $80
      And I expect to see $81 equal $01
      And I expect to see $2000 equal $81
      And I expect to see $2001 equal $02
      And I expect to see $2002 equal $03
      And I expect to see $2003 equal $04
      And I expect to see t_t1 equal $03
      And I expect to see t_t2 equal $04
      And I expect to see t_t3 equal $05
      And I expect to see t_t4 equal $06
      And I expect to see t_t5 equal $07
      And I expect to see t_t6 equal $08
      And I expect to see t_t7 equal $09
      And I expect to see t_t8 equal $0a
      And I expect to see t_t9 equal $0b
      And I expect to see t_t10 equal $0c
