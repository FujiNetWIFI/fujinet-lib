Feature: library test - apple2 sp_find_device

  This tests fujinet-network apple2 sp_find_device

  Scenario: execute apple2 sp_find_device for name that does not exist returns 0 for ID
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add file for compiling "features/apple2/fn_network/invokers/test_sp_find_unmatched.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
     When I execute the procedure at _init for no more than 5000 instructions
      And I print ascii from _sp_payload to _sp_payload+$20

     # number of devices in slot 1 is 6 from emulator
     And I expect to see _sp_payload equal 6

     # return from sp_find_device is 0 when device does not match
     And I expect register A equal 0
     And I expect register X equal 0
