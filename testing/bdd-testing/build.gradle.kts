plugins {
    kotlin("jvm") version "1.8.21"
    application
}

repositories {
    mavenLocal()
    mavenCentral()
}

dependencies {
    implementation("fujinet:fujinet-bdd:1.0.0-SNAPSHOT")
}

kotlin {
    jvmToolchain(11)
}

tasks {
    register("runFeatures", JavaExec::class) {
        mainClass.set("bdd6502.RunFeatureTests")
        classpath = sourceSets["main"].runtimeClasspath
        jvmArgs = listOf(
            "--add-opens", "java.base/java.util=ALL-UNNAMED",
            "--add-opens", "java.base/java.lang.reflect=ALL-UNNAMED",
            "--add-opens", "java.base/java.text=ALL-UNNAMED",
            "--add-opens", "java.desktop/java.awt.font=ALL-UNNAMED",
        )
    }

    register("runFeaturesWithTrace", JavaExec::class) {
        mainClass.set("bdd6502.RunFeatureTests")
        classpath = sourceSets["main"].runtimeClasspath
        jvmArgs = listOf(
            "-Dbdd6502.trace=true",
            "--add-opens", "java.base/java.util=ALL-UNNAMED",
            "--add-opens", "java.base/java.lang.reflect=ALL-UNNAMED",
            "--add-opens", "java.base/java.text=ALL-UNNAMED",
            "--add-opens", "java.desktop/java.awt.font=ALL-UNNAMED",
        )
    }
}

application {
    // Run the main server, then connect to the webpage to run tests etc.
    mainClass.set("MainKt")
    applicationDefaultJvmArgs = listOf(
        "-Dbdd6502.trace=false",
        "-Dcom.replicanet.cukesplus.server.featureEditor",
        // "-Dcom.replicanet.ACEServer.debug.requests=", // enable this to get more debug output from server
        "--add-opens", "java.base/java.util=ALL-UNNAMED",
        "--add-opens", "java.base/java.lang.reflect=ALL-UNNAMED",
        "--add-opens", "java.base/java.text=ALL-UNNAMED",
        "--add-opens", "java.desktop/java.awt.font=ALL-UNNAMED",
    )
}