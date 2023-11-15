Feature: IO library test - fn_io_get_adapter_config

  This tests FN-IO fn_io_get_adapter_config

  Scenario: execute _fn_io_get_adapter_config
    Given atari-fn-io simple test setup
      And I add common atari-io files
      And I add atari src file "fn_io/fn_io_get_adapter_config.s"
      And I add file for compiling "features/atari/fn_io/stubs/bus-adapter-config.s"
      And I create and load simple atari application
      And I set register A to $00
      And I set register X to $A0
      And I write memory at $80 with $00

     When I execute the procedure at _fn_io_get_adapter_config for no more than 1010 instructions
      And I print ascii from $A000 to $A000+144

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $e8
     And I expect to see DSTATS equal $40
     And I expect to see DBYTLO equal 140
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $00
     And I expect to see DAUX2 equal $00
     And I expect to see DBUFLO equal lo($A000)
     And I expect to see DBUFHI equal hi($A000)

    # Test the return values at A/X point to a struct with correct data
    Then string at $A000 contains
    """
      33:ssid name!!
      64:the 'hostname'
       4:ip
       4:gw
       4:nm
       4:dns
       6:macadd
       6:bssid
      15:version string
    """