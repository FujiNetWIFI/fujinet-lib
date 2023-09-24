package TestGlue

import cucumber.api.java.en.Given

class MathSteps {
    @Throws(Exception::class)
    @Given("^I add (\\d+) to property \"([^\"]*)\"$")
    fun `i add n bytes to property`(n: Int, propName: String) {
        val v = System.getProperty(propName).toInt()
        System.setProperty(propName, "${v + n}")
    }
}