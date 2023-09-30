Feature: library test - apple2 network_open

  This tests fujinet-network apple2 network_open

  Scenario: execute apple2 _network_open
    Given apple2-fn-nw application test setup
      And I add common apple2-sp files
      And I add apple2 src file "network_open.s"
      And I add common src file "network_unit.s"
      And I add file for compiling "features/apple2/invokers/test_network_open.s"
      And I add file for compiling "../../apple2/src/sp_find_network.s"
      And I create and load apple-single application using crt-file "features/apple2/stubs/crt0.s"
      And I write string "n5:foo" as ascii to memory address $a012
      And I write word at t_devicespec with hex $a012
      And I write memory at t_mode with $04
      And I write memory at t_translate with $80
     When I execute the procedure at _init for no more than 1300 instructions

    Then I expect register A equal 0
     And I expect register X equal 0
     And I expect to see t_cb_executed equal 1
     # CMD is OPEN (4)
     And I expect to see spe_cmd equal 4
     # DEST is smartport NETWORK device (3)
     And I expect to see spe_dest equal 3
     # 'O' control code
     And I expect to see _sp_cmdlist+4 equal 79

     # validate payload
     #  [0] = strlen+2
     #  [1] = 0
     #  [2] = mode
     #  [3] = translation
     #  [4..4+len] = devicespec
     #  [..+1] = 0x9b
     #  ... = $00
    When I hex+ dump memory between _sp_payload and _sp_payload+11
    # ascii chars are "n5:foo": 6e:n, 35:5, 3a::, 66:f, 6f:o, 6f:o
    Then property "test.BDD6502.lastHexDump" must contain string ": 08 00 04 80 6e 35 3a 66  6f 6f 9b :"
