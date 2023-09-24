package util

import TestGlue.MemorySteps

sealed class ScreenToken {
    abstract fun code(): Int
}

open class EscToken(
    private val code: Int
): ScreenToken() {
    override fun code(): Int = code
}

object EscEsc: EscToken(0x5b)
object UpArrow: EscToken(0x5c)
object DownArrow: EscToken(0x5d)
object LeftArrow: EscToken(0x5e)
object RightArrow: EscToken(0x5f)
object InvEsc: EscToken(253)
object Delete: EscToken(254)
object Insert: EscToken(255)

data class ScreenChar(
    val c: Char,
    val isInverse: Boolean = false,
    val isCtrl: Boolean = false,
): ScreenToken() {
    private val code = convertToCode(this)

    override fun code(): Int {
        return code
    }

    companion object {
        fun convertToCode(t: ScreenChar): Int {
            if (!t.isInverse && !t.isCtrl) return MemorySteps.charToInternal(t.c)
            if (t.isInverse && !t.isCtrl) return MemorySteps.charToInternal(t.c) or 0x80

            // ctrl-, ctrl-a ctrl-b is 64, 65, ... internal, up to ctrl-Z
            // ctrl-. is 96

            val i = when(t.c) {
                ',' -> 64
                in 'a' .. 'z' -> 65 + t.c.code - 'a'.code
                '.' -> 96
                ';' -> 123
                else -> throw Exception("unknown token: c: ${t.c}, inv: ${t.isInverse}, ctl: true")
            }

            return if (t.isInverse) (i or 0x80) else i
        }
    }
}