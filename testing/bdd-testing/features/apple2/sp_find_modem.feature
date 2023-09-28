Feature: library test - apple2 spn_find_modem

  This tests fujinet-network apple2 spn_find_modem

  Scenario: execute sp.c
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add file for compiling "features/apple2/invokers/test_sp_find_modem.s"
      And I add file for compiling "../../apple2/src/spn_find_modem.s"
      And I create and load apple-single application
     When I execute the procedure at _init for no more than 1300 instructions

    # offset 5 contains the device name
    Then string at _sp_payload+5 contains
     """
     0:MODEM
     """
     # number of devices in slot 1 is 6 (FUJINET_DISK_0/PRINTER/NETWORK/FN_CLOCK/MODEM/CPM)
     And I expect to see _sp_payload equal 6
     # length of the device name
     And I expect to see _sp_payload+4 equal 5

     # return from _sp_find_modem is 5 for Device number
     And I expect register A equal 5
     And I expect register X equal 0
