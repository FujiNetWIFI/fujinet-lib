Feature: IO library test - fn_fuji_error

  This tests FN-FUJI fn_fuji_error

  Scenario Outline: execute fn_fuji_error should return appropriate values depending on DSTATS
    Given atari-fn-fuji simple test setup
      And I add atari src file "fn_fuji/fn_fuji_error.s"
      And I create and load simple atari application

     When I write memory at DSTATS with <init>
      And I execute the procedure at _fn_fuji_error for no more than 10 instructions
     Then I expect register A equal <A>
     Then I expect register X equal <X>

    Examples:
      | init |  A   |  X   |
      | 0x00 | 0x00 | 0x00 |
      | 0x01 | 0x00 | 0x00 |
      | 0x7f | 0x00 | 0x00 |
      | 0x80 | 0x01 | 0x00 |
      | 0x81 | 0x01 | 0x00 |
      | 0xff | 0x01 | 0x00 |