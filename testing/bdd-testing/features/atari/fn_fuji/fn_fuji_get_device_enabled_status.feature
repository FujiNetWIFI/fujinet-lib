Feature: IO library test - fn_fuji_get_device_enabled_status

  This tests FN-FUJI fn_fuji_get_device_enabled_status

  Scenario: execute _fn_fuji_get_device_enabled_status should set A
    Given atari-fn-fuji simple test setup
      And I add atari src file "fn_fuji/fn_fuji_get_device_enabled_status.s"
      And I create and load simple atari application
      And I set register A to $aa

     When I execute the procedure at _fn_fuji_get_device_enabled_status for no more than 5 instructions

     Then I expect register A equal 1