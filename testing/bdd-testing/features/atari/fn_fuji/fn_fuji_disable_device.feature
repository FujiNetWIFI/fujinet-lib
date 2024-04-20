Feature: IO library test - fn_fuji_disable_device

  This tests FN-FUJI fn_fuji_disable_device is a NO-OP

  Scenario: execute _fuji_disable_device
    Given atari-fn-fuji simple test setup
      And I add atari src file "fn_fuji/fuji_disable_device.s"
      And I create and load simple atari application
      And I execute the procedure at _fuji_disable_device for no more than 1 instructions
