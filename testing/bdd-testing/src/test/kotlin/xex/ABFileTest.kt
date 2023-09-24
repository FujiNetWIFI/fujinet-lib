package xex

import org.junit.jupiter.api.Test
import util.resourceStream

class ABFileTest {
    @Test
    fun `can read defender batshit xex`() {
        val stream = resourceStream("/Defender.xex")
        val xex = ABFile(stream.readAllBytes())
        xex.dump()
    }
}