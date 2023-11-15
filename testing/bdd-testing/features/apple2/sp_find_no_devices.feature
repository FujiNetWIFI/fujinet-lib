Feature: library test - apple2 sp_find_device

  This tests fujinet-network apple2 sp_find_device

  Scenario: execute apple2 sp_find_device returns 0 when there are no devices
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add file for compiling "features/apple2/invokers/test_sp_no_devices.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
     When I execute the procedure at _init for no more than 5000 instructions
      And I print ascii from _sp_payload to _sp_payload+$20

     # number of devices in slot 1 is 0 in this test
     And I expect to see _sp_payload equal 0

     # return from _sp_find_network is 0 for no devices
     And I expect register A equal 0
     And I expect register X equal 0
