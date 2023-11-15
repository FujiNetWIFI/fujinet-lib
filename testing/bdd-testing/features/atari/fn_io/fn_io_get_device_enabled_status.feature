Feature: IO library test - fn_io_get_device_enabled_status

  This tests FN-IO fn_io_get_device_enabled_status

  Scenario: execute _fn_io_get_device_enabled_status should set A
    Given atari-fn-io simple test setup
      And I add atari src file "fn_io/fn_io_get_device_enabled_status.s"
      And I create and load simple atari application
      And I set register A to $aa

     When I execute the procedure at _fn_io_get_device_enabled_status for no more than 5 instructions

     Then I expect register A equal 1