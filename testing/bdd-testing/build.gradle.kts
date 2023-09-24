plugins {
    kotlin("jvm") version "1.8.21"
    application
}

group = "net.fish.bdd6502"
version = "1.0-SNAPSHOT"

val assertJVersion: String by project
val mockkVersion: String by project
val junitJupiterEngineVersion: String by project

repositories {
    mavenLocal()
    mavenCentral()
}

dependencies {
    implementation("BDD6502:BDD6502:1.0.9-SNAPSHOT")

    // fuck hamcrest. We're going jupiter/assertj baby.
    implementation("org.junit.jupiter:junit-jupiter-api:$junitJupiterEngineVersion")
    implementation("org.junit.jupiter:junit-jupiter-params:$junitJupiterEngineVersion")
    implementation("org.junit.jupiter:junit-jupiter-engine:$junitJupiterEngineVersion")
    implementation("org.assertj:assertj-core:$assertJVersion")
    implementation("io.mockk:mockk:$mockkVersion")
    // testImplementation(kotlin("test"))
}

configurations {
    testCompileOnly {
        // GET THEE HENCE JUNIT4
        exclude(module = "junit", group = "junit")
    }
}

kotlin {
    jvmToolchain(11)
}

tasks {
    test {
        useJUnitPlatform()
    }

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