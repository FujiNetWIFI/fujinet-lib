Feature: library test - apple2 sp_find_fuji

  This tests fujinet-network apple2 sp_find_fuji

  Scenario: execute apple2 sp_find_fuji
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add file for compiling "features/apple2/invokers/test_sp_find_fuji.s"
      And I add file for compiling "../../apple2/src/sp_find_fuji.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
     When I execute the procedure at _init for no more than 575 instructions
     # And I print ascii from _sp_payload+5 to _sp_payload+$20

    # offset 5 contains the device name
    Then string at _sp_payload+5 contains
     """
     0:FUJINET_DISK_0
     """
     # number of devices in slot 1 is 6 (FUJINET_DISK_0/NETWORK/PRINTER/FN_CLOCK/MODEM/CPM)
     And I expect to see _sp_payload equal 6
     # length of the device name
     And I expect to see _sp_payload+4 equal 14

     # return from _sp_find_fuji is 1 for Device number
     And I expect register A equal 1
     And I expect register X equal 0
