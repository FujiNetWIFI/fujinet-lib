import com.replicanet.cukesplus.Main

// This is the gradle "application" run target
// And will create a server/web interface at http://localhost:8001/ace-builds-master/demo/autocompletion.html
// Run with:
//
// ./gradlew run

// To create an IntelliJ Run configuration for this, add following VM args:
// -Dbdd6502.trace=true -Dcom.replicanet.cukesplus.server.featureEditor --add-opens java.base/java.util=ALL-UNNAMED --add-opens java.base/java.lang.reflect=ALL-UNNAMED --add-opens java.base/java.text=ALL-UNNAMED --add-opens java.desktop/java.awt.font=ALL-UNNAMED
// These are already added to the "application" section of build.gradle.kts

fun main() {
    val mainArgs = "--tags ~@ignore --monochrome --plugin pretty --plugin html:target/cucumber --plugin json:target/report1.json --glue macros --glue TestGlue features".split(" ").toTypedArray()
    Main.main(mainArgs)
}
