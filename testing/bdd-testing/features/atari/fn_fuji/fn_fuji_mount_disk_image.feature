Feature: IO library test - fn_fuji_mount_disk_image

  This tests FN-FUJI fn_fuji_mount_disk_image

  Scenario: execute _fn_fuji_mount_disk_image successfully
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fn_fuji_mount_disk_image.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_bb.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      # slot, mode
      And I write memory at t_b1 with $05
      And I write memory at t_b2 with $01
      And I write word at t_fn with address _fn_fuji_mount_disk_image
     When I execute the procedure at _init for no more than 95 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $fe
     And I expect to see DCOMND equal $f8
     And I expect to see DSTATS equal $00
     And I expect to see DBYTLO equal $00
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $05
     And I expect to see DAUX2 equal $01
     And I expect to see DBUFLO equal $00
     And I expect to see DBUFHI equal $00
    
    # Check return value in A/X
     And I expect register A equal $00
     And I expect register X equal $00

    # check BUS was called
    Then I expect to see $80 equal $01

  Scenario: execute _fn_fuji_mount_disk_image with error
    Given atari-fn-fuji application test setup
      And I add common atari-io files
      And I add atari src file "fn_fuji/fn_fuji_mount_disk_image.s"
      And I add file for compiling "features/atari/fn_fuji/test-apps/test_bb.s"
      And I add file for compiling "features/atari/fn_fuji/stubs/bus-simple-err.s"
      And I create and load atari application
      And I write memory at $80 with $ff
      # slot, mode
      And I write memory at t_b1 with $05
      And I write memory at t_b2 with $01
      And I write memory at t_bus_err with $69
      And I write word at t_fn with address _fn_fuji_mount_disk_image
     When I execute the procedure at _init for no more than 95 instructions

    # check the DCB values were set correctly
    Then I expect to see DDEVIC equal $70
     And I expect to see DUNIT equal $01
     And I expect to see DTIMLO equal $fe
     And I expect to see DCOMND equal $f8
     And I expect to see DSTATS equal $69
     And I expect to see DBYTLO equal $00
     And I expect to see DBYTHI equal $00
     And I expect to see DAUX1 equal $05
     And I expect to see DAUX2 equal $01
     And I expect to see DBUFLO equal $00
     And I expect to see DBUFHI equal $00
    
    # Check return value in A/X
     And I expect register A equal $69
     And I expect register X equal $00

    # check BUS was called
    Then I expect to see $80 equal $01
