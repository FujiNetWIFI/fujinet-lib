Feature: library test - apple2 sp_init

  This tests fujinet-network apple2 sp_init

  Scenario: execute apple2 sp_init
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add file for compiling "features/apple2/invokers/test_sp_init.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
     When I execute the procedure at _init for no more than 100 instructions

     # return from _sp_init is 1 in A, and 0 in X
     And I expect register A equal 1
     And I expect register X equal 0
