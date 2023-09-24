package TestGlue

import cucumber.api.java.en.Given
import org.assertj.core.api.Assertions.assertThat

class CpuSteps {
    @Throws(Exception::class)
    @Given("^I expect register state (.*)$")
    fun `i expect register state`(statusCmd: String) {
        // allow register checking with an input of the format:
        // Z:1       # Z flag is on (0 for off)
        // Combinations separated by commas:
        // N:0,Z:0   # N,Z flags both off
        // This allows us to test single or multiple status flags directly instead of the "include"/"exclude" steps

        val machine = Glue.getMachine()
        val flagToOnOff = statusCmd.split(",").fold(mutableMapOf<Char, Boolean>()) { m, cmd ->
            // convert FLAG:0/1 into map entry
            // Flags are: N,C,Z,I,D,V
            val parts = cmd.split(":")
            // only use first character of the Left side, and only match 0 for false, everything else is true.
            m[parts[0][0]] = parts[1] != "0"
            m
        }
        println("Testing status flags against: $flagToOnOff")

        flagToOnOff.forEach { (r, onOff) ->
            when(r) {
                'N' -> assertThat(machine.cpu.negativeFlag).isEqualTo(onOff)
                'C' -> assertThat(machine.cpu.carryFlag).isEqualTo(onOff)
                'Z' -> assertThat(machine.cpu.zeroFlag).isEqualTo(onOff)
                'I' -> assertThat(machine.cpu.irqDisableFlag).isEqualTo(onOff)
                'D' -> assertThat(machine.cpu.decimalModeFlag).isEqualTo(onOff)
                'V' -> assertThat(machine.cpu.overflowFlag).isEqualTo(onOff)
                else -> throw Exception("Unknown register: $r in statusCmd: $statusCmd")
            }
        }
    }

    @Throws(Exception::class)
    @Given("^I convert registers (.*) to address$")
    fun `I convert registers to address`(regs: String) {
        // Input format: "AX", "XY" etc. Convert to word value, e.g. "XY" -> X + 256 * Y
        // Also can have single char, which acts like standard version
        val v = regsToAddress(regs)
        System.setProperty("test.BDD6502.regsValue", "$v")
    }

    companion object {
        fun regsToAddress(regs: String): Int {
            if (regs.length > 2) throw Exception("Maximum of 2 registers supported, found: >$regs<")
            if (regs.any { !setOf('A', 'X', 'Y').contains(it.uppercaseChar()) }) throw Exception("Registers must be from A, X, or Y")
            if (regs.isEmpty()) {
                return 0
            }
            val machine = Glue.getMachine()
            val aR = machine.cpu.accumulator
            val xR = machine.cpu.xRegister
            val yR = machine.cpu.yRegister

            val mapRegToValue = mapOf('A' to aR, 'X' to xR, 'Y' to yR)

            var v = mapRegToValue.getOrDefault(regs[0], 0)
            if (regs.length == 2) v += 256 * mapRegToValue.getOrDefault(regs[1], 0)

            return v
        }
    }
}