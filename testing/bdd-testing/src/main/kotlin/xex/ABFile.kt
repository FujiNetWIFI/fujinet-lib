package xex

import java.lang.Exception
import kotlin.math.min

// Atari Binary file, e.g. xex, obx, com.
data class ABFile(val bytes: ByteArray) {
    val sections = mutableListOf<Section>()
    var runAddress: Int = 0

    init {
        // read the data and extract all the sections
        // check header is FFFF for now
        if (bytes[0] != 0xff.toByte() || bytes[1] != 0xff.toByte()) {
            val headerAsHex = bytes.sliceArray(0..1).joinToString(separator = "") { eachByte -> "%02x".format(eachByte) }
            throw Exception("Unhandled header type: $headerAsHex")
        }

        var i = 0
        while (i < bytes.size) {
            if (bytes[i] == 0xe2.toByte() && bytes[i+1] == 0x02.toByte()) {
                // INIT, skip 4 bytes [0xe2 0x02 0xe3 0x02]
                i += 4
                val initAddress = bytes[i].toUByte().toInt() + 256 * bytes[i+1].toUByte().toInt()
                sections += InitSection(initAddress)
                i += 2
            } else if (bytes[i] == 0xe0.toByte() && bytes[i+1] == 0x02.toByte()) {
                // RUN ADDR, skip 4 bytes [0xe0 0x02 0xe1 0x02]
                i += 4
                runAddress = bytes[i].toUByte().toInt() + 256 * bytes[i+1].toUByte().toInt()
                i += 2
                // should now be at the end
                if (i != bytes.size) throw Exception("Failed to process file, got run address but there is more data, i: $i, size: ${bytes.size}")
            } else {
                // new data block. Header is optional (except first, but this works either way)
                if (bytes[i] == 0xff.toByte() && bytes[1] == 0xff.toByte()) {
                    i += 2
                }
                val startAddress = bytes[i].toUByte().toInt() + 256 * bytes[i+1].toUByte().toInt()
                val endAddress = bytes[i+2].toUByte().toInt() + 256 * bytes[i+3].toUByte().toInt()
                val blockLen = endAddress - startAddress + 1
                i += 4
                sections += DataSection(startAddress, bytes.sliceArray(i until i + blockLen))
                i += blockLen
            }
        }
    }

    fun dump() {
        println("ABFile, runAddress: 0x${runAddress.toString(16)}, len: 0x${bytes.size.toString(16)}")
        sections.forEach { section ->
            when(section) {
                is DataSection -> {
                    val upTo7 = min(7, section.data.size - 1)
                    val first8 = section.data.sliceArray(0..upTo7).joinToString(separator = " ") { eachByte -> "%02x".format(eachByte) }
                    println("  DataSection, start: 0x${section.startAddress.toString(16)}, len: 0x${section.data.size.toString(16)}: $first8")
                }

                is InitSection -> {
                    println("  InitSection, init: 0x${section.initAddress.toString(16)}")
                }
            }
        }
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as ABFile

        return bytes.contentEquals(other.bytes)
    }

    override fun hashCode(): Int {
        return bytes.contentHashCode()
    }

}

sealed class Section

data class InitSection(
    val initAddress: Int
) : Section()

data class DataSection(
    val startAddress: Int,
    val data: ByteArray
): Section() {
    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as DataSection

        if (startAddress != other.startAddress) return false
        if (!data.contentEquals(other.data)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = startAddress
        result = 31 * result + data.contentHashCode()
        return result
    }
}