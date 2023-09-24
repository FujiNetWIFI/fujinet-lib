package TestGlue

import cucumber.api.java.en.Given

class DebugSteps {

    // This is just "I hex dump memory between ..." repeated but not using Scenario
    @Throws(Exception::class)
    @Given("^I print memory from (.*) to (.*)$")
    fun `i print memory`(start: String, end: String) {
        println(MemorySteps.memoryHex(start, end, Glue.getMachine()))
    }

    @Throws(Exception::class)
    @Given("^I print ascii from (.*) to (.*)$")
    fun `i print ascii`(start: String, end: String) {
        println(MemorySteps.memoryHex(start, end, Glue.getMachine(), false))
    }
}