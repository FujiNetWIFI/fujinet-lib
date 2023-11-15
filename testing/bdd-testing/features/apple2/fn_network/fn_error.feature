Feature: library test - apple2 fn_error

  This tests fujinet-network apple2 fn_error

  Scenario Outline: execute apple2 _fn_error stores and converts device error code to library code 
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add file for compiling "features/test-setup/test-apps/test_b.s"
      And I create and load apple-single application using crt-file "features/apple2/fn_network/stubs/crt0.s"
      And I write memory at t_b1 with <device_error>
      And I write word at t_fn with address _fn_error
     When I execute the procedure at _init for no more than 100 instructions

    Then I expect register A equal <library_error>
     And I expect register X equal 0
     And I expect to see _fn_device_error equal <device_error>

   Examples:
   | device_error         | library_error   |
   | $80                  | FN_ERR_UNKNOWN  |
   | $ff                  | FN_ERR_UNKNOWN  |
   | SP_ERR_OK            | FN_ERR_OK       |
   | SP_ERR_BAD_CMD       | FN_ERR_BAD_CMD  |
   | SP_ERR_BAD_PCNT      | FN_ERR_BAD_CMD  |
   | SP_ERR_BAD_UNIT      | FN_ERR_BAD_CMD  |
   | SP_ERR_BAD_CTRL      | FN_ERR_BAD_CMD  |
   | SP_ERR_BAD_CTRL_PARM | FN_ERR_BAD_CMD  |
   | SP_ERR_BUS_ERR       | FN_ERR_IO_ERROR |
   | SP_ERR_IO_ERROR      | FN_ERR_IO_ERROR |
   | SP_ERR_NO_DRIVE      | FN_ERR_IO_ERROR |
   | SP_ERR_NO_WRITE      | FN_ERR_IO_ERROR |
   | SP_ERR_BAD_BLOCK     | FN_ERR_IO_ERROR |
   | SP_ERR_DISK_SW       | FN_ERR_IO_ERROR |
   | SP_ERR_DEV_SPEC0     | FN_ERR_IO_ERROR |
   | SP_ERR_DEV_SPECF     | FN_ERR_IO_ERROR |
   | SP_ERR_NON_FATAL50   | FN_ERR_WARNING  |
   | SP_ERR_NON_FATAL7F   | FN_ERR_WARNING  |
   | SP_ERR_OFFLINE       | FN_ERR_OFFLINE  |