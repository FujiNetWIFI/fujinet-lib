Feature: IO library test - fn_io_disable_device

  This tests FN-IO fn_io_disable_device is a NO-OP

  Scenario: execute _fn_io_disable_device
    Given atari-fn-io simple test setup
      And I add atari src file "fn_io/fn_io_disable_device.s"
      And I create and load simple atari application
      And I execute the procedure at _fn_io_disable_device for no more than 1 instructions
