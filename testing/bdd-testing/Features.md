# Feature conditions

The following lists was extracted using:

```shell
# My list:
$ grep '^ *@.*"' TestGlue/*.kt | cut -d\" -f2- | sed 's/")$//' | sort

# Core:
$ grep '^ *@.*"' Glue.java | cut -d\" -f2- | sed 's/")$//' | sort
```

## Custom Given/When/Then

```text
^I add (\\d+) to property \"([^\"]*)\"$
^I add compiler option \"([^\"]*)\"$
^I add file for compiling \"([^\"]*)\"$
^I convert registers (.*) to address$
^I convert vice-labels file \"([^\"]*)\" to acme labels file \"([^\"]*)\"$
^I create and load application$
^I create and load simple application$
^I create or clear directory \"([^\"]*)\"$
^I expect register state (.*)$
^I fill memory from (.*) to (.*) with (.*)$
^I hex dump memory for (.*) bytes from property \"([^\"]*)\"$
^I hex\\+ dump ascii between (.*) and (.*)$
^I hex\\+ dump memory between (.*) and (.*)$
^I load xex \"([^\"]*)\"$
^I patch machine from bin file \"([^\"]*)\"$
^I print ascii from (.*) to (.*)$
^I print memory from (.*) to (.*)$
^I set label (.*) to registers address (.*)$
^I start compiling for (.*) in \"([^\"]*)\" with config \"([^\"]*)\"$
^I write encoded string \"([^\"]*)\" to (.*)$
^I write string \"([^\"]*)\" as ascii to memory address (.*)$
^I write string \"([^\"]*)\" as internal to memory address (.*)$
^I write word at (.*) with address (.*)$
^I write word at (.*) with hex (.*)$
^I write word at (.*) with value (.*)$
^memory at ([^\\s]*) contains$
^memory at registers (.*) contains$
^screen memory at (.*) contains ascii$
^string at ([^\\s]*) contains$
^string at registers (.*) contains$
```

## BDD6502 Given/When/Then

```text
^a CHARGEN ROM from file \"([^\"]*)\"$
^a new audio expansion$
^a new C64 video display$
^a new video display with 16 colours$
^a new video display with overscan and 16 colours$
^a new video display$
^a ROM from file \"([^\"]*)\" at (.*)$
^a simple user port to 24 bit bus is installed$
^a user port to 24 bit bus is installed$
^add a 2-to-1 merge layer with registers at '(.*)'$
^add a Chars layer with registers at '(.*)' and addressEx '(.*)'$
^add a Chars V4.0 layer with registers at '(.*)' and screen addressEx '(.*)' and planes addressEx '(.*)'$
^add a GetBackground layer fetching from layer index '(.*)'$
^add a Mode7 layer with registers at '(.*)' and addressEx '(.*)'$
^add a Sprites layer with registers at '(.*)' and addressEx '(.*)'$
^add a Sprites2 layer with registers at '(.*)' and addressEx '(.*)' and running at (.*)MHz$
^add a Sprites2 layer with registers at '(.*)' and addressEx '(.*)'$
^add a Sprites3 layer with registers at '(.*)' and addressEx '(.*)'$
^add a StaticColour layer for palette index '(.*)'$
^add a Tiles layer with registers at '(.*)' and screen addressEx '(.*)' and planes addressEx '(.*)'$
^add a Vector layer with registers at '(.*)' and addressEx '(.*)'$
^add C64 hardware$
^APU clock divider (\\d+)$
^APU memory clock divider (\\d+)$
^assert on exec memory from (.+) to (.+)$
^assert on read memory from (.+) to (.+)$
^assert on write memory from (.+) to (.+)$
^assert that \"([^\"]*)\" is false$
^assert that \"([^\"]*)\" is true$
^audio refresh is independent$
^audio refresh window every (.*) instructions$
^automation click current menu item \"([^\"]*)\"$
^automation expand main menu item \"([^\"]*)\"$
^automation find window from pattern \"([^\"]*)\"$
^automation focus window$
^automation wait for idle$
^automation wait for window close$
^C64 video display does not save debug BMP images$
^C64 video display saves debug BMP images to leaf filename \"([^\"]*)\"$
^clear all external devices$
^close current file$
^connect to remote monitor at TCP \"([^\"]*)\" port \"([^\"]*)\"$
^disable debug pixel picking$
^disconnect remote monitor$
^display until window closed$
^enable APU mode$
^enable debug pixel picking$
^enable remote debugging$
^enable user port bus debug output$
^enable video display bus debug output$
^expect end of file$
^expect image \"([^\"]*)\" to be identical to \"([^\"]*)\"$
^expect the line to contain \"([^\"]*)\"$
^expect the next line to contain \"([^\"]*)\"$
^fill data byte '(.*)' to 24bit bus at '(.*)' to '(.*)' stride '(.*)' and addressEx '(.*)'$
^force C64 displayed bank to (\\d+)$
^I am using C64 processor port options$
^I assert the following bytes are the same$
^I assert the following hex bytes are the same$
^I assert the uninitialised memory read flag is clear$
^I assert the uninitialised memory read flag is set$
^I compare memory range (.+)-(.+) to (.+)$
^I continue executing the procedure for no more than (.+) instructions until PC = (.+)$
^I continue executing the procedure for no more than (.+) instructions$
^I continue executing the procedure until return or until PC = (.+)$
^I continue executing the procedure until return$
^I continue executing until (.+) = (.+)$
^I create file \"(.*?)\" with$
^I disable trace (byte|word) at (.+)$
^I disable trace$
^I disable uninitialised memory read protection$
^I enable trace (byte|word) at (.+)$
^I enable trace with indent$
^I enable trace$
^I enable uninitialised memory read protection with immediate fail$
^I enable uninitialised memory read protection$
^I execute the indirect procedure at (.+) until return or until PC = (.+)$
^I execute the indirect procedure at (.+) until return$
^I execute the procedure at (.+) for no more than (.+) instructions until PC = (.+)$
^I execute the procedure at (.+) for no more than (.+) instructions$
^I execute the procedure at (.+) until return$
^I expect IODevice buffer to equal \"(.+)\"$
^I expect memory (.+) to contain memory (.+)$
^I expect memory (.+) to equal memory (.+)$
^I expect memory (.+) to exclude memory (.+)$
^I expect register (.+) contain (.+)$
^I expect register (.+) equal (.+)$
^I expect register (.+) exclude (.+)$
^I expect register (.+) to be greater than (.+)$
^I expect register (.+) to be less than (.+)$
^I expect the cycle count to be no more than (.+) cycles$
^I expect to see (.+) contain (.+)$
^I expect to see (.+) equal (.+)$
^I expect to see (.+) exclude (.+)$
^I expect to see (.+) greater than (.+)$
^I expect to see (.+) less than (.+)$
^I fill memory with (.+)$
^I have a simple 6502 system$
^I have a simple overclocked 6502 system$
^I hex dump memory between (.+) and (.+)$
^I install PrintIODevice at (.+)$
^I load bin \"(.*?)\" at (.+)$
^I load crt \"(.*?)\"$
^I load labels \"(.*?)\"$
^I load prg \"(.*?)\"$
^I push a (.+) byte to the stack$
^I reset the cycle count$
^I reset the uninitialised memory read flag$
^I run the command line ignoring return code: (.*)$
^I run the command line: (.*)$
^I set label (.+) equal to (.+)$
^I set register (.+) to (.+)$
^I setup a (.+) byte stack slide$
^I start comparing memory at (.+)$
^I start writing memory at (.+)$
^I write memory at (.+) with (.+)$
^I write the following bytes$
^I write the following hex bytes$
^ignore address (.+) to (.+) for trace$
^Joystick (\\d+) is (NONE|U|D|L|R|UL|UR|DL|DR|FIRE|UFIRE|ULFIRE|URFIRE|DFIRE|DLFIRE|DRFIRE|LFIRE|RFIRE)$
^limit video display to (.*) fps$
^open file \"([^\"]*)\" for reading$
^processing each line in file \"([^\"]*)\" and only output to file \"([^\"]*)\" lines after finding a line containing \"([^\"]*)\"$
^processing each line in file \"([^\"]*)\" and only output to file \"([^\"]*)\" lines that do not contain any lines from \"([^\"]*)\"$
^profile clear$
^profile print$
^profile start$
^profile stop$
^property \"([^\"]*)\" is set to string \"([^\"]*)\"$
^property \"([^\"]*)\" must contain string \"([^\"]*)\"$
^property \"([^\"]*)\" must contain string ignoring whitespace \"([^\"]*)\"$
^randomly initialise all memory using seed (.*)$
^remote monitor continue without waiting$
^remote monitor wait for (.*) hits$
^remote monitor wait for hit$
^render (.*) video display frames$
^render a C64 video display frame$
^render a video display frame$
^render a video display until H=(.*) and V=(.*)$
^render a video display until vsync$
^rendering the video until window closed$
^send remote monitor command \"([^\"]*)\" \"([^\"]*)\"$
^send remote monitor command \"([^\"]*)\"$
^send remote monitor command without parsing \"(.*)\"$
^show C64 video window$
^show video window$
^skip line$
^starting an automation process \"([^\"]*)\" with parameters \"([^\"]*)\"$
^starting an automation process \"([^\"]*)\" with parameters: (.*)$
^That does exit on BRK$
^That does fail on BRK$
^the layer has 16 colours$
^the layer has overscan$
^unlimit video display fps$
^Until (.+) = (.+) execute from (.+)$
^video display add CIA1 timers with raster offset (\\d+) , (\\d+)$
^video display add joystick to port 1$
^video display add joystick to port 2$
^video display does not save debug BMP images$
^video display processes (.*) pixels per instruction$
^video display refresh window every (.*) instructions$
^video display saves debug BMP images to leaf filename \"([^\"]*)\"$
^wait for (\\d+) milliseconds$
^wait for debugger command$
^wait for debugger connection$
^write data byte '(.*)' to 24bit bus at '(.*)' and addressEx '(.*)'$
^write data from file \"([^\"]*)\" to 24bit bus at '(.*)' and addressEx '(.*)'$
```