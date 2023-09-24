package testglue

import TestGlue.MemorySteps
import org.assertj.core.api.Assertions.assertThat
import org.junit.jupiter.api.Test
import util.*

class MemoryStepsTest {
    @Test
    fun `list of chars can be converted to string`() {
        val chars = listOf('h', 'e', 'l', 'l', 'o')
        assertThat(chars.joinToString("")).isEqualTo("hello")
    }

    @Test
    fun `should parse double escape`() {
        val s = "{esc}{esc}"
        assertThat(MemorySteps.toTokens(s)).containsExactly(EscEsc)
    }

    @Test
    fun `should parse arrows`() {
        val s = "{esc}{up}{esc}{down}{esc}{left}{esc}{right}"
        assertThat(MemorySteps.toTokens(s)).containsExactly(UpArrow, DownArrow, LeftArrow, RightArrow)
    }

    @Test
    fun `should parse other esc chars`() {
        val s = "{esc}{ins}{esc}{del}{esc}{^}2"
        assertThat(MemorySteps.toTokens(s)).containsExactly(Insert, Delete, InvEsc)
    }

    @Test
    fun `should parse simple string`() {
        assertThat(MemorySteps.toTokens("a1!@[ ]")).containsExactly(
            ScreenChar('a'),
            ScreenChar('1'),
            ScreenChar('!'),
            ScreenChar('@'),
            ScreenChar('['),
            ScreenChar(' '),
            ScreenChar(']')
        )
    }

    @Test
    fun `should parse combined inverse and normal chars`() {
        assertThat(MemorySteps.toTokens("{inv}a{inv}b {inv}    {inv}")).containsExactly(
            ScreenChar('a', isInverse = true),
            ScreenChar('b'),
            ScreenChar(' '),
            ScreenChar(' ', isInverse = true),
            ScreenChar(' ', isInverse = true),
            ScreenChar(' ', isInverse = true),
            ScreenChar(' ', isInverse = true),
        )
    }

    @Test
    fun `should parse ctrl chars mixed with other chars`() {
        assertThat(MemorySteps.toTokens("{^}..{^},,{^};;")).containsExactly(
            ScreenChar('.', isCtrl = true),
            ScreenChar('.'),
            ScreenChar(',', isCtrl = true),
            ScreenChar(','),
            ScreenChar(';', isCtrl = true),
            ScreenChar(';'),
        )
    }

    @Test
    fun `should parse all types of chars`() {
        assertThat(MemorySteps.toTokens("X{inv}ABC{inv}{esc}{esc}{inv}{esc}{^}2DEF{inv}X")).containsExactly(
            ScreenChar('X'),
            ScreenChar('A', isInverse = true),
            ScreenChar('B', isInverse = true),
            ScreenChar('C', isInverse = true),
            EscEsc,
            InvEsc,
            ScreenChar('D', isInverse = true),
            ScreenChar('E', isInverse = true),
            ScreenChar('F', isInverse = true),
            ScreenChar('X'),
        )
    }

    @Test
    fun `code() should return internal screen code`() {
        assertThat(ScreenChar('a').code()).isEqualTo(0x61)
        assertThat(ScreenChar('a', isInverse = true).code()).isEqualTo(0xe1)
        assertThat(ScreenChar('a', isCtrl = true).code()).isEqualTo(0x41)
        assertThat(ScreenChar('A').code()).isEqualTo(0x21)
        assertThat(ScreenChar('A', isInverse = true).code()).isEqualTo(0xa1)
        assertThat(ScreenChar('1').code()).isEqualTo(0x11)
        assertThat(ScreenChar('1', isInverse = true).code()).isEqualTo(0x91)
        assertThat(ScreenChar(' ').code()).isEqualTo(0x00)
        assertThat(ScreenChar(' ', isInverse = true).code()).isEqualTo(0x80)
        assertThat(ScreenChar('!').code()).isEqualTo(0x01)
        assertThat(ScreenChar('!', isInverse = true).code()).isEqualTo(0x81)
        assertThat(ScreenChar('_').code()).isEqualTo(0x3f)
        assertThat(ScreenChar('_', isInverse = true).code()).isEqualTo(0xbf)
        assertThat(ScreenChar(',', isCtrl = true).code()).isEqualTo(0x40)
        assertThat(ScreenChar(';', isCtrl = true).code()).isEqualTo(123)
        assertThat(EscEsc.code()).isEqualTo(0x5b)
        assertThat(UpArrow.code()).isEqualTo(0x5c)
        assertThat(DownArrow.code()).isEqualTo(0x5d)
        assertThat(LeftArrow.code()).isEqualTo(0x5e)
        assertThat(RightArrow.code()).isEqualTo(0x5f)
        // are these correct codes? need to double-check
        assertThat(InvEsc.code()).isEqualTo(253)
        assertThat(Delete.code()).isEqualTo(254)
        assertThat(Insert.code()).isEqualTo(255)
    }

}
