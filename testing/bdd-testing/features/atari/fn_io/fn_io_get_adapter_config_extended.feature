Feature: IO library test - fn_io_get_adapter_config

  This tests FN-IO fn_io_get_adapter_config

  Scenario: execute _fn_io_get_adapter_config_extended
    Given atari-fn-io simple test setup
      And I add common atari-io files
      And I add atari src file "fn_io/fn_io_get_adapter_config_extended.s"
      And I add atari src file "fn_io/fn_io_get_adapter_config.s"
      And I add file for compiling "features/atari/fn_io/stubs/bus-adapter-config-ext.s"
      And I create and load simple atari application
      And I set register A to $00
      And I set register X to $A0
      And I write memory at $80 with $00

     When I execute the procedure at _fn_io_get_adapter_config_extended for no more than 1900 instructions
      And I print ascii from $A000 to $A000+244

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $0f
     And I expect to see DCOMND equal $e8
     And I expect to see DSTATS equal $40
     And I expect to see DBYTLO equal 240
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $01
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
      16:100.100.100.100
      16:101.101.101.101
      16:99.99.99.99
      16:102.102.102.102
      18:00:11:22:33:44:55
      18:aa:bb:cc:dd:ee:ff
    """