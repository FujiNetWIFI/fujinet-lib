package TestGlue

import cucumber.api.java.en.Given
import org.apache.commons.io.FileUtils
import java.nio.file.Paths
import kotlin.io.path.createDirectories

class FileSteps {
    @Given("^I create or clear directory \"([^\"]*)\"$")
    @Throws(Exception::class)
    fun `i create directory`(dir: String) {
        val cwd = Paths.get(".")
        val newDir = cwd.resolve(dir)
        FileUtils.deleteDirectory(newDir.toFile())
        newDir.createDirectories()
    }
}