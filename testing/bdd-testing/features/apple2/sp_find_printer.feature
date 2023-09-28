Feature: library test - apple2 sp_find_printer

  This tests fujinet-network apple2 sp_find_printer

  Scenario: execute sp.c
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add file for compiling "features/apple2/invokers/test_sp_find_printer.s"
      And I add file for compiling "../../apple2/src/sp_find_printer.s"
      And I create and load apple-single application
     When I execute the procedure at _init for no more than 720 instructions

    # offset 5 contains the device name
    Then string at _sp_payload+5 contains
     """
     0:PRINTER
     """
     # number of devices in slot 1 is 6 (FUJINET_DISK_0/PRINTER/NETWORK/FN_CLOCK/MODEM/CPM)
     And I expect to see _sp_payload equal 6
     # length of the device name
     And I expect to see _sp_payload+4 equal 7

     # return from _sp_find_printer is 2 for Device number
     And I expect register A equal 2
     And I expect register X equal 0
