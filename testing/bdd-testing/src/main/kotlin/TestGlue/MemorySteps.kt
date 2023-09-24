package TestGlue

import com.loomcom.symon.machines.Machine
import cucumber.api.java.en.Given
import org.assertj.core.api.Assertions.assertThat
import util.*
import kotlin.io.path.absolutePathString
import kotlin.io.path.createTempFile
import kotlin.io.path.deleteIfExists
import kotlin.io.path.writeText

class MemorySteps {
    @Throws(Exception::class)
    @Given("^I fill memory from (.*) to (.*) with (.*)$")
    fun `i fill memory from A to B with`(start: String, end: String, v: String) {
        val startAddress = Glue.valueToInt(start)
        val endAddress = Glue.valueToInt(end)
        val value = Glue.valueToInt(v)
        val count = endAddress - startAddress + 1

        val machine = Glue.getMachine()
        repeat(count) { i ->
            machine.bus.write(startAddress + i, value)
        }
    }
    @Throws(Exception::class)
    @Given("^I hex dump memory for (.*) bytes from property \"([^\"]*)\"$")
    fun `i hex dump memory for n bytes from property`(nBytes: String, propName: String) {
        val startAddress = System.getProperty(propName).toInt()
        val endAddress = startAddress + nBytes.toInt()
        val hex = memoryHex("$startAddress", "$endAddress", Glue.getMachine())
        System.setProperty("test.BDD6502.lastHexDump", hex)
    }

    @Throws(Exception::class)
    @Given("^I hex\\+ dump memory between (.*) and (.*)$")
    fun `i hex dump memory between X and Y improved`(start: String, end: String) {
        val hex = memoryHex(start, end, Glue.getMachine())
        System.setProperty("test.BDD6502.lastHexDump", hex)
        Glue.getGlue().scenario.write(hex)
    }

    @Throws(Exception::class)
    @Given("^I hex\\+ dump ascii between (.*) and (.*)$")
    fun `i hex dump ascii between X and Y improved`(start: String, end: String) {
        val hex = memoryHex(start, end, Glue.getMachine(), false)
        System.setProperty("test.BDD6502.lastHexDump", hex)
        Glue.getGlue().scenario.write(hex)
    }

    @Throws(Exception::class)
    @Given("^I write word at (.*) with hex (.*)$")
    fun `i write word at with hex`(mem: String, hexString: String) {
        var hex = hexString
        if (hex.startsWith('$')) {
            hex = hex.substring(1)
        }
        if (hex.length > 4 || hex.isEmpty()) throw Exception("hex specified cannot be written to word: >$hex<")

        val address = Glue.valueToInt(mem)

        var nHex = hex // normalise to 2 or 4 bytes
        if (hex.length == 1 || hex.length == 3) nHex = "0${hex}"
        val hi = if (nHex.length == 2) 0 else nHex.substring(0, 2).toInt(16)
        val loIndex = if (nHex.length == 4) 2 else 0
        val lo = nHex.substring(loIndex, loIndex + 2).toInt(16)

        println("MemorySteps::I write word at with hex: mem: $mem, hex: $hex, address: ${address.toString(16)}, lo: ${lo.toString(16)}, hi: ${hi.toString(16)}")
        val machine = Glue.getMachine()
        machine.bus.write(address, lo)
        machine.bus.write(address+1, hi)
    }

    @Throws(Exception::class)
    @Given("^I write word at (.*) with value (.*)$")
    fun `i write word at with value`(mem: String, v: String) {
        val address = Glue.valueToInt(mem)
        val value = v.toInt()
        val lo = value % 256
        val hi = value / 256
        val machine = Glue.getMachine()
        machine.bus.write(address, lo)
        machine.bus.write(address+1, hi)
    }

    @Throws(Exception::class)
    @Given("^I write word at (.*) with address (.*)$")
    fun `i write word at with address`(mem: String, v: String) {
        val address = Glue.valueToInt(mem)
        val value = Glue.valueToInt(v)
        val lo = value % 256
        val hi = value / 256
        val machine = Glue.getMachine()
        machine.bus.write(address, lo)
        machine.bus.write(address+1, hi)
    }

    @Throws(Exception::class)
    @Given("^memory at registers (.*) contains$")
    fun `memory at registers contains`(regs: String, structData: String) {
        assertMemoryMatches(structData, Glue.getMachine(), CpuSteps.regsToAddress(regs))
    }

    @Throws(Exception::class)
    @Given("^memory at ([^\\s]*) contains$")
    fun `memory at contains`(location: String, structData: String) {
        assertMemoryMatches(structData, Glue.getMachine(), Glue.valueToInt(location))
    }

    @Throws(Exception::class)
    @Given("^string at registers (.*) contains$")
    fun `string at registers contains`(regs: String, structData: String) {
        assertMemoryMatches(structData, Glue.getMachine(), CpuSteps.regsToAddress(regs), false)
    }

    @Throws(Exception::class)
    @Given("^string at ([^\\s]*) contains$")
    fun `string at contains`(location: String, structData: String) {
        assertMemoryMatches(structData, Glue.getMachine(), Glue.valueToInt(location), false)
    }

    private fun assertMemoryMatches(structData: String, machine: Machine, address: Int, isInternal: Boolean = true) {
        // each line contains an offset to add after the test, and a string to check at start of current address
        // e.g.
        // 33:ssid here
        // 15:another string

        var mutableAddress = address
        structData.lines().forEach { line ->
            val parts = line.split(":")
            val offset = parts[0].trim().toInt()
            val testString = parts[1].trim()
            val memString = testString.indices.map { i ->
                val mem = machine.cpu.bus.read(mutableAddress + i)
                if (isInternal) internalToChar(mem) else Char(mem)
            }.joinToString("")
            assertThat(memString).isEqualTo(testString)

            mutableAddress += offset
        }
    }

    @Throws(Exception::class)
    @Given("^I set label (.*) to registers address (.*)$")
    fun `I set label to registers address`(label: String, regs: String) {
        val address = CpuSteps.regsToAddress(regs)
        val tempFile = createTempFile()
        tempFile.writeText("$label =$address")
        Glue.loadLabels(tempFile.absolutePathString())
        tempFile.deleteIfExists()
    }

    @Throws(Exception::class)
    @Given("^I write string \"([^\"]*)\" as ascii to memory address (.*)$")
    fun `i write string as ascii to memory address`(s: String, adr: String) {
        val scenario = Glue.getGlue().scenario
        val address = Glue.valueToInt(adr)
        scenario.write("Writing string \"$s\" to address: 0x${address.toString(16)} as ascii")
        // Should write (at)ascii, not internal, as space is 00, which is null terminator too!
        val machine = Glue.getMachine()
        s.forEachIndexed { i, c ->
            machine.bus.write(address + i, c.code)
        }
        // end with nul (0) char to terminate string
        machine.bus.write(address + s.length, 0)
    }

    @Throws(Exception::class)
    @Given("^I write string \"([^\"]*)\" as internal to memory address (.*)$")
    fun `i write string as internal to memory address`(s: String, adr: String) {
        val scenario = Glue.getGlue().scenario
        val address = Glue.valueToInt(adr)
        scenario.write("Writing string \"$s\" to address: 0x${address.toString(16)} as internal")
        // Should write internal, as space is 00, which is null terminator too!
        val machine = Glue.getMachine()
        s.forEachIndexed { i, c ->
            machine.bus.write(address + i, charToInternal(c))
        }
    }

    @Throws(Exception::class)
    @Given("^I write encoded string \"([^\"]*)\" to (.*)$")
    fun `i write encoded string to`(s: String, adr: String) {
        // no bounds checking done, so write tests carefully
        val address = Glue.valueToInt(adr)
        val machine = Glue.getMachine()
        val tokens = toTokens(s)
        tokens.forEachIndexed { i, token ->
            machine.bus.write(address + i, token.code())
        }
    }

    @Throws(Exception::class)
    @Given("^screen memory at (.*) contains ascii$")
    fun `screen memory at X contains ascii data`(adr: String, s: String) {
        val scenario = Glue.getGlue().scenario
        val address = Glue.valueToInt(adr)
        scenario.write("Reading ascii screen from address: 0x${address.toString(16)}")
        val machine = Glue.getMachine()

        // loop through all the tokens generated from the given string and match their codes to what's in memory
        var currentLocation = address
        // need 4 backslashes to reduce to 1!
        // Remove any continuation + (CR)LF so we can have strings over multiple lines in test, but treat as continuous
        val tokens = toTokens(s)
        tokens.forEach { t ->
            val sV = machine.bus.read(currentLocation)
            val sA = internalToChar(sV)
            val tC = t.code()
            val tA = internalToChar(tC)
            if (sV != tC) {
                val l = currentLocation - address
                val x = l % 40
                val y = l / 40
                throw Exception("""
                    Failed to match location ${currentLocation.toString(16)}, offset: ${l.toString(16)} (x: $x, y: $y)
                    Found screen code: $sV (ascii: $sA)
                        Expected code: $tC (ascii: $tA)
                """.trimIndent())
            }
            currentLocation++
        }
    }

    companion object {
        fun memoryHex(start: String, end: String, machine: Machine, isInternal: Boolean = true): String {
            var currentAddress: Int = Glue.valueToInt(start)
            val addrEnd: Int = Glue.valueToInt(end)

            var cr = 0
            var hexOutput = ""
            var section = ""
            while (currentAddress < addrEnd) {
                if (cr == 0) {
                    hexOutput += String.format("%2s:", Integer.toHexString(currentAddress).replace(' ', '0'))
                }
                if (cr == 8) {
                    hexOutput += " "
                }
                val v: Int = machine.bus.read(currentAddress, false)
                val hex = String.format("%2s", Integer.toHexString(v)).replace(' ', '0')
                section += if (isInternal) internalToChar(v) else Char(v)
                hexOutput += " $hex"
                cr += 1
                if (cr >= 16) {
                    hexOutput += " : >$section<\n"
                    cr = 0
                    section = ""
                }
                currentAddress++
            }

            if (section.isNotEmpty()) {
                hexOutput += " : >$section<"
            }
            return hexOutput
        }

        fun toTokens(s: String): List<ScreenToken> {
            // remove any line extensions (backslash followed by CR/LF) so we can use multiple lines in test but access continuous memory
            val crRemoved = s.replace("\\\\\r?\n".toRegex(), "")

            // Using altirra encoding of screen data.
            // {inv} toggles inverse mode
            // {^}   marks next char as ctrl- (always lower case after if letter, e.g. {^}a for t-bar char)
            // {esc} escapes the next token
            // e.g.
            // {esc}{esc}{inv}{esc}{^}2{inv}a{^}ab
            // is
            // <ESC char>, <inverse on>, <esc>ctrl-2 (the curly arrow up-left), <inverse off>, a, ctrl-a, b

            val tokens = mutableListOf<ScreenToken>()

            var readingMode = false
            var modeString = ""
            var isInverse = false
            var isEscape = false
            var isCtl = false

            crRemoved.forEach { c ->
                when(c) {
                    '{' -> {
                        if (readingMode) throw Exception("Found '{' before a matching '}' from previous open.")
                        // start reading a mode string
                        readingMode = true
                        modeString = ""
                    }
                    '}' -> {
                        // finished reading mode string. process it.
                        readingMode = false
                        when (modeString) {
                            "esc" -> {
                                if (isEscape) {
                                    // double escape, add the token
                                    tokens += EscEsc
                                }
                                isEscape = !isEscape
                            }
                            "inv" -> isInverse = !isInverse
                            "^" -> isCtl = !isCtl
                            "up", "down", "right", "left", "del", "ins" -> {
                                if (!isEscape) throw Exception("received {$modeString} but not after {esc} - don't know what to print")
                                isEscape = false
                                when (modeString) {
                                    "up" -> tokens += UpArrow
                                    "down" -> tokens += DownArrow
                                    "left" -> tokens += LeftArrow
                                    "right" -> tokens += RightArrow
                                    "del" -> tokens += Delete
                                    "ins" -> tokens += Insert
                                }
                            }
                            else -> throw Exception("Unknown mode: $modeString")
                        }
                    }
                    else -> {
                        // a normal character pressed
                        if (readingMode) {
                            // keep building the mode string up until hit closing curly brace char
                            modeString = "${modeString}$c"
                        } else {
                            if (isEscape) {
                                if (isCtl && c == '2') {
                                    tokens += InvEsc
                                    isEscape = false
                                    isCtl = false
                                } else {
                                    // this is an error, we don't have any {esc} + char we can process other than ctrl-2, they are all {esc} + another mode, e.g. {esc}{up}
                                    throw Exception("In escape mode, and received normal character: $c, should not be possible")
                                }
                            } else {
                                // Add our character and any current Inverse or Ctrl mode
                                tokens += ScreenChar(c, isInverse, isCtl)
                                // ctl is always 1 char, but inverse is sticky
                                isCtl = false
                            }
                        }
                    }
                }
            }

            return tokens
        }


        fun internalToChar(v: Int): Char {
            return when (v) {
                in 0..63 -> internalToCharMap.getOrDefault(v, '.')
                in 97..122 -> 'a' + v - 97
                else -> '.'
            }
        }

        fun charToInternal(c: Char): Int = charToInternalMap.getOrDefault(c, 0)

        private val internalToCharMap = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_".mapIndexed { i, c -> i to c }.toMap().toMutableMap().let { m ->
            (97 .. 122).forEach { i -> m[i] = Char(i) } // add 'a..z'
            m
        }.toMap()
        private val charToInternalMap = internalToCharMap.entries.associate { (i, c) -> c to i }
        private val defaultCode = charToInternalMap['.']!!
    }
}