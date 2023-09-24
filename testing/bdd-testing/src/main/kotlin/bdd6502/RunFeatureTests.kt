@file:JvmName("RunFeatureTests")
package bdd6502

import com.replicanet.cukesplus.Main

fun main() {
    val mainArgs = "--plugin pretty --plugin html:target/cucumber --plugin json:target/report1.json --glue macros --glue TestGlue features".split(" ").toTypedArray()
    Main.main(mainArgs)
}

